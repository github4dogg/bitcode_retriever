#include "macho_retriever.h"
#include "macho_util.h"

#include <string.h>

int main(int argc, char *argv[]) {
  int extract = 0;
  const char *filename;
  if (argc > 2) {
    extract = strcmp(argv[1], "-extract") == 0;
    filename = argv[2];
  } else {
    filename = argv[1];
  }

  FILE *stream = fopen(filename, "rb");

  struct bitcode_archive *archives[max_number_of_archives()];
  int archive_count;
  retrieve_bitcode(stream, archives, &archive_count);

  for (int i = 0; i < archive_count; i++) {
    if (archives[i]) {
      if (extract) {
        char *bitcode_files[1024];
        int bitcode_count;
        write_to_bitcode(archives[i], bitcode_files, &bitcode_count);
        for (int j = 0; j < bitcode_count; j++) {
          printf("%s\n", bitcode_files[j]);
          free(bitcode_files[j]);
        }
      } else {
        char *xar_name = write_to_xar(archives[i]);
        printf("%s\n", xar_name);
        free(xar_name);
      }
      free(archives[i]->buffer);
      free(archives[i]);
    }
  }

  fclose(stream);
  return 0;
}
