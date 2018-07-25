#include "data/blob.h"

Blob* lovrBlobCreate(void* data, size_t size, const char* name) {
  Blob* blob = lovrAlloc(Blob, lovrBlobDestroy);
  if (!blob) return NULL;

  blob->data = data;
  blob->size = size;
  blob->name = name;

  return blob;
}

void lovrBlobDestroy(void* ref) {
  Blob* blob = ref;
  free(blob->data);
  free(blob);
}
