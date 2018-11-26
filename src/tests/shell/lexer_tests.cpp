#undef __debug__

#include <phlib/istream.h>
#include <shell/lexer.h>
#include <chrono>
#include <stdio.h>

using namespace phlib;
using namespace psh;
using namespace std::chrono;

int test1() {
    const char *src_buffer = "ident(\"1.22\");if{};";
    auto input_stream = new IStream(src_buffer, 20, false);
    auto lexer = new Lexer(input_stream);
    int counter = 0;
    while (true) {
        Token *token = lexer->get_next_token();
        if (token == nullptr) break;
        printf("token lexed : %s\n", token->token_to_string());
        delete token;
    }
    delete lexer;
    delete input_stream;
    return 0;
}

int test2() {
    const int size = 10000;
    char *src_buffer = (char *) phlib::malloc(size);
    for (int i = 0; i < size; i++) {
        src_buffer[i] = 'a' + (char) (i % ('z' - 'a'));
        if (i % 10 == 0)
            src_buffer[i] = ';';
    }
    src_buffer[size - 1] = '\0';
    auto input_stream = new IStream(src_buffer, size, false);
    auto lexer = new Lexer(input_stream);
    int counter = 0;
    high_resolution_clock::time_point start_time = high_resolution_clock::now();
    while (true) {
        Token *token = lexer->get_next_token();
        if (token == nullptr) break;
        delete token;
    }
    high_resolution_clock::time_point end_time = high_resolution_clock::now();
    delete lexer;
    delete input_stream;
    phlib::free(src_buffer);
    duration<double> time_span = duration_cast<duration<double>>(end_time - start_time);
    printf("lexing took %f seconds\n", time_span.count());
    return 0;
}

int main(int argc, const char **argv) {
    int ret = 0;
    if ((ret = test1()) != 0) return ret;
    if ((ret = test2()) != 0) return ret;
    return ret;
}