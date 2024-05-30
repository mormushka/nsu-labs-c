#ifndef PROGRESS_BAR_H
#define PROGRESS_BAR_H

#include <stdio.h>

#define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
#define PBWIDTH 60

void print_progress(double percentage);
size_t file_size(FILE *f);

#endif
