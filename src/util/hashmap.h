/**
 * @headerfile
 * @author Ilya Potemin <potemin1999@bk.ru>
 * @date 8/14/19
 * This file is part of Phantom Shell project,
 * which is child project of Phantom OS.
 * GNU Lesser General Public License v3.0
 */

#ifndef PHANTOMSHELL_HASHMAP_H
#define PHANTOMSHELL_HASHMAP_H

#include "types.h"

typedef void *any_t;

typedef void *map_t;

typedef void *map_iterator_t;

map_t hashmap_new();

int hashmap_delete(map_t m);

map_iterator_t hashmap_iterator_new(map_t m);

int hashmap_iterator_delete(map_iterator_t iter);

int hashmap_put(map_t m, const char *key, any_t value);

int hashmap_get(map_t m, const char *key, any_t *out_value);

int hashmap_remove(map_t m, const char *key);

size_t hashmap_size(map_t m);

uint32_t string_hash(const char *str);

#endif //PHANTOMSHELL_HASHMAP_H
