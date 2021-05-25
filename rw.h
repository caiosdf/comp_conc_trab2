#ifndef RW_H
#define RW_H

//função para realizar a entrada do leitor de maneira que seja thread safety
void reader_in(int id);

//função para realizar a saída do leitor de maneira que seja thread safety
void reader_out(int id);

//função para realizar a entrada do escritor de maneira que seja thread safety
void writer_in(int id);

//função para realizar a saída do escritor de maneira que seja thread safety
void writer_out(int id);

#endif