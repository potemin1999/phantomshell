/**
 * @file
 * @author Ilya Potemin <potemin1999@bk.ru>
 * @date 8/21/19
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#include "lib.h"
#include "vm/vm.h"
#include "compiler.h"

map_t global_func_map;

const struct func_desc_t *compiler_define_func(ast_node_decl_func_t *func_node) {
    if (!global_func_map) {
        global_func_map = hashmap_new();
    }
    struct func_desc_t *func;
    int ret = hashmap_get(global_func_map, func_node->name, (any_t) &func);
    if (ret != -3) {
        //TODO: print an error
        compiler_panic("function is already defined : %s", func_node->name);
    }
    func = malloc(sizeof(struct func_desc_t));
    ast_node_decl_var_t *current = func_node->args->first;
    func->name = strdup(func_node->name);
    func->args_count = 0;
    while (current) {
        func->args_size += 4;
        func->args_count++;
        current = current->next;
    }
    char *signature_buffer = malloc(1536);
    char *buffer_ptr = signature_buffer;
    bzero(signature_buffer, 1536);
    strcpy(buffer_ptr, func_node->name);
    buffer_ptr += strlen(func_node->name);

    strcpy(buffer_ptr++, "(");
    func->args = malloc(sizeof(struct func_arg_t) * func->args_count);
    current = func_node->args->first;
    for (size_t i = 0; i < func->args_count; i++) {
        func->args[i].name = current->var_name;
        func->args[i].type = current->var_type;
        char *type_signature = type_signature_by_name(func->args[i].type);
        size_t type_signature_len = strlen(type_signature);
        strcpy(buffer_ptr, type_signature);
        free(type_signature);
        buffer_ptr += type_signature_len;
        current = current->next;
    }
    strcpy(buffer_ptr++, ")");

    func->ret_type = strdup(func_node->ret_type);
    if (func->ret_type) {
        char *type_signature = type_signature_by_name(func->ret_type);
        size_t type_signature_len = strlen(type_signature);
        strcpy(buffer_ptr, type_signature);
        free(type_signature);
        buffer_ptr += type_signature_len;
    }
    func->signature = signature_buffer;
    ret = hashmap_put(global_func_map, func->name, func);
    if (ret != 0) {
        compiler_panic("function definition failed : %s", func->name);
    }
    return func;
}

const struct func_desc_t *compiler_lookup_func_by_name(const char *name) {
    if (!global_func_map) {
        global_func_map = hashmap_new();
    }
    struct func_desc_t *func = 0;
    int ret = hashmap_get(global_func_map, name, (any_t) &func);
    if (ret != 0) {
        //TODO: print error message
        compiler_panic("function lookup failed : %s", name);
    }
    return func;
}

int compile_func(struct scope_handler_t *scope, ast_node_decl_func_t *node) {
    struct bytecode_emitter_t body_emitter = compiler_emitter_buffered_new(256);
    struct scope_handler_t func_scope;
    struct scope_var_t func_local_vars[128];
    func_scope.vars = func_local_vars;
    func_scope.vars_cap = 128;
    func_scope.vars_off = 0;
    func_scope.vars_max_size = 0;
    func_scope.vars_size = 0;
    func_scope.stack_size = 0;
    func_scope.stack_cap = INT32_MAX;
    //TODO: calculate stack max size
    func_scope.stack_max_size = 512;
    func_scope.parent = 0;
    func_scope.emitter = &body_emitter;
    func_scope.is_function_scope = 1;

    const struct func_desc_t *func_desc = compiler_define_func(node);
    for (int i = 0; i < func_desc->args_count; i++) {
        static_type_t arg_type = static_type_by_name(func_desc->args[i].type);
        frame_define_var(&func_scope, arg_type, func_desc->args[i].name);
    }
    compile_scope(&func_scope, node->body);

    uint16_t vars_size = func_scope.vars_max_size;
    uint16_t stack_size = func_scope.stack_max_size;
    size_t body_size = body_emitter.size;
    ubyte_t *exec_data = (ubyte_t *) malloc(body_size + 4);
    vars_size = htobe16(vars_size);
    stack_size = htobe16(stack_size);
    memcpy(exec_data + 0, &vars_size, 2);
    memcpy(exec_data + 2, &stack_size, 2);
    compiler_emitter_sink(&body_emitter, exec_data + 4, body_size);
    compiler_emitter_buffered_destroy(&body_emitter);
    frame_destroy_scope(&func_scope);

    size_t signature_len = strlen(func_desc->signature);
    uint16_t signature_index;
    const_pool_register_value(func_desc->signature, &signature_index);

    if (scope->parent == 0) {
        //TODO: make proper check to the root scope
        vm_pool_const_t index = 0;
        vm_func_handle_t *func_handle = 0;
        vm_register_constant(signature_len, func_desc->signature, &index);
        vm_register_function(index, body_size + 4, exec_data, &func_handle);
        return 0;
    } else {
        compiler_panic("unable to compile global func node: non root scope");
    }
}