#include "codegen_utils.h"

int codegen_sizeof(header_data *hdata, int n, 
  char *prefix, char *suffix, FILE *fp) {

  int i;
  int k;
  int j;
  int ndefs;
  struct_data *defs;
  char *base;
  
  for (k = 0; k < n; k++) {
    defs = hdata[k].defs;
    ndefs = hdata[k].def_count;
    for (i = j = 0; i < ndefs; i++) {
      if (is_prefix(defs[i].info.name,prefix) && is_suffix(defs[i].info.name,suffix)) {
        base = basename(defs[i].filename);
        if (j == 0) {
          fprintf(fp,"#include \"%s\"\n",base);
          j++;
        } else {
          if (strcmp(base,basename(defs[i-1].filename))) {
            fprintf(fp,"#include \"%s\"\n",base);
          }
        }
      }
    }
  }

  fprintf(fp,"\nint main(int argc, char *argv[]) {\n\n");

  for (k = 0; k < n; k++) {
    defs = hdata[k].defs;
    ndefs = hdata[k].def_count;
    for (i = 0; i < ndefs; i++) {
      if (is_prefix(defs[i].info.name,prefix) && is_suffix(defs[i].info.name,suffix)) {
        if (defs[i].info.is_typedef) {
          fprintf(fp,"  %s %s_actual;\n",defs[i].info.name,defs[i].info.name);
        } else {
          fprintf(fp,"  struct %s %s_actual;\n",defs[i].info.name,defs[i].info.name);
        }
      }
    }
  }

  fprintf(fp,"\n");

  for (k = 0; k < n; k++) {
    defs = hdata[k].defs;
    ndefs = hdata[k].def_count;
    for (i = 0; i < ndefs; i++) {
      if (is_prefix(defs[i].info.name,prefix) && is_suffix(defs[i].info.name,suffix)) {
        fprintf(fp,"  printf(\"\\nName: %s\\nSize: %%d\\n\",sizeof(%s_actual));\n",defs[i].info.name,defs[i].info.name);
      }
    }
  }

  fprintf(fp,"\n}\n");

  return 0;
}
