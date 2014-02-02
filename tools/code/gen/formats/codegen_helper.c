#include "codegen_utils.h"

static void codegen_helpers_prototype(struct_data *sdata, char *helper_type, FILE *out) {
  if (sdata->info.type == ST_UNION) {
    if (sdata->info.is_typedef) {
      fprintf(out,"int %s_%s(%s *st, int idx)",
        sdata->info.name,helper_type,sdata->info.name);
    } else {
      fprintf(out,"int %s_%s(union %s *st, int idx)",
        sdata->info.name,helper_type,sdata->info.name);
    }
  } else {
    if (sdata->info.is_typedef) {
      fprintf(out,"int %s_%s(%s *st)",
        sdata->info.name,helper_type,sdata->info.name);
    } else {
      fprintf(out,"int %s_%s(struct %s *st)",
        sdata->info.name,helper_type,sdata->info.name);
    }
  }
}

static void codegen_patch_prototype(struct_data *sdata, FILE *out) {
  if (sdata->info.is_typedef) {
    fprintf(out,"int %s_patch_apply(%s *info, %s_patch *patch)",
      sdata->info.name,sdata->info.name,sdata->info.name);
  } else {
    fprintf(out,"int %s_patch_apply(struct %s *info, %s_patch *patch)",
      sdata->info.name,sdata->info.name,sdata->info.name);
  }
}

static void codegen_address_to_patch_prototype(struct_data *sdata, FILE *out) {
  fprintf(out,"kr_value *%s_address_to_patch(%s_patch *patch, kr_address2 *addr)",
    sdata->info.name,sdata->info.name);
}

static void codegen_helpers_union_content_from_type(struct_data *def, 
  char *fun_name, FILE *out) {
  int i;

  for (i = 0; i < def->info.member_count; i++) {
    if (memb_struct_check(&def->info.members[i])) {
      fprintf(out,"    case %d: {\n",i);
      fprintf(out,"      %s_%s(&st->%s);\n",
        def->info.members[i].type_info.substruct_info.type_name,
        fun_name,def->info.members[i].name);
      fprintf(out,"      break;\n    }\n");
    }
    /* TODO: handle unions of primitives (ints,floats,strings...) */
  }
        
  return;
}

static void codegen_helper_init(member_info *memb, int i, 
  char *indent, char *arr, FILE *out) {

  switch (memb->type) {
    case T_CHAR: {
      if (memb->arr || (memb->ptr == 1)) {
        // TO-DO 
      }
      break;
    }
    case T_INT32: {
      fprintf(out,"%s  st->%s%s = %d;\n",
        indent,memb->name,arr,memb->type_info.int32_info.init);
      break;
    }
    case T_INT64: {
      fprintf(out,"%s  st->%s%s = %" PRId64 ";\n",
        indent,memb->name,arr,memb->type_info.int64_info.init);
      break;
    }
    case T_UINT32: {
      fprintf(out,"%s  st->%s%s = %u;\n",
        indent,memb->name,arr,memb->type_info.uint32_info.init);
      break;
    }
    case T_UINT64: {
      fprintf(out,"%s  st->%s%s = %" PRIu64 ";\n",
        indent,memb->name,arr,memb->type_info.uint64_info.init);
      break;
    }
    case T_FLOAT: {
      fprintf(out,"%s  st->%s%s = %0.2f;\n",
        indent,memb->name,arr,memb->type_info.float_info.init);
      break;
    }
    case T_DOUBLE: {
      fprintf(out,"%s  st->%s%s = %0.2f;\n",
        indent,memb->name,arr,memb->type_info.double_info.init);
      break;
    }
    default : break;
  }

  return;
}

static void codegen_helper_random(member_info *memb, int i, 
  char *indent, char *arr, FILE *out) {

  switch (memb->type) {
    case T_CHAR: {
      if (memb->arr) {
        fprintf(out,"    scale = (double)%d / RAND_MAX;\n",abs(97-122)); /* downcased ascii letters only */
        fprintf(out,"    st->%s[i] = %d + floor(rand() * scale);\n",memb->name,97);
        fprintf(out,"    if (i == %d) {\n      st->%s[%d] = '\\0';\n    }\n",
          memb->type_info.char_info.max,memb->name,memb->type_info.char_info.max);
      }
      break;
    }
    case T_INT32: {
      fprintf(out,"%s  scale = (double)%d / RAND_MAX;\n",indent,
        abs(memb->type_info.int32_info.min-memb->type_info.int32_info.max));
      fprintf(out,"%s  st->%s%s = %d + floor(rand() * scale);\n",
        indent,memb->name,arr,memb->type_info.int32_info.min);
      break;
    }
    case T_INT64: {
      fprintf(out,"%s  scale = (double)%" PRId64 " / RAND_MAX;\n",indent,
        labs(memb->type_info.int64_info.min-memb->type_info.int64_info.max));
      fprintf(out,"%s  st->%s%s = %" PRId64 " + floor(rand() * scale);\n",
        indent,memb->name,arr,memb->type_info.int64_info.min);
      break;
    }
    case T_UINT32: {
      fprintf(out,"%s  scale = (double)%u / RAND_MAX;\n",indent,
        abs(memb->type_info.uint32_info.min-memb->type_info.uint32_info.max));
      fprintf(out,"%s  st->%s%s = %u + floor(rand() * scale);\n",
        indent,memb->name,arr,memb->type_info.uint32_info.min);
      break;
    }
    case T_UINT64: {
      fprintf(out,"%s  scale = (double)%" PRIu64 " / RAND_MAX;\n",indent,
        labs(memb->type_info.uint64_info.min-memb->type_info.uint64_info.max));
      fprintf(out,"%s  st->%s%s = %" PRIu64 " + floor(rand() * scale);\n",
        indent,memb->name,arr,memb->type_info.uint64_info.min);
      break;
    }
    case T_FLOAT: {
      fprintf(out,"%s  scale = (double)%0.2f / RAND_MAX;\n",indent,
        fabs(memb->type_info.float_info.min-memb->type_info.float_info.max));
      fprintf(out,"%s  st->%s%s = %0.2f + floor(rand() * scale);\n",
        indent,memb->name,arr,memb->type_info.float_info.min);
      break;
    }
    case T_DOUBLE: {
      fprintf(out,"%s  scale = (double)%0.2f / RAND_MAX;\n",indent,
        fabs(memb->type_info.double_info.min-memb->type_info.double_info.max));
      fprintf(out,"%s  st->%s%s = %0.2f + floor(rand() * scale);\n",
        indent,memb->name,arr,memb->type_info.double_info.min);
      break;
    }
    default : break;
  }

  return;

}

static void codegen_helper_valid(member_info *memb, int i, 
  char *indent, char *arr, FILE *out) {

  switch (memb->type) {
    case T_CHAR: {
      if (memb->arr) {
        fprintf(out,"    if (!st->%s%s) {\n      break;\n    }\n    if (i == %d && st->%s%s) {\n      return %d;\n    }\n",
          memb->name,arr,memb->type_info.char_info.max,memb->name,arr,(i + 2) * -1);
      }
      break;
    }
    case T_INT32: {
      fprintf(out,"%s  if ( (st->%s%s < %d) || (st->%s%s > %d) ) {\n%s    return %d;\n  %s}\n\n",
        indent,memb->name,arr,memb->type_info.int32_info.min,
        memb->name,arr,memb->type_info.int32_info.max,indent,(i + 2) * -1,indent);
      break;
    }
    case T_INT64: {
      fprintf(out,"%s  if ( (st->%s%s < %" PRId64 ") || (st->%s%s > %" PRId64 ") ) {\n%s    return %d;\n  %s}\n\n",
        indent,memb->name,arr,memb->type_info.int64_info.min,
        memb->name,arr,memb->type_info.int64_info.max,indent,(i + 2) * -1,indent);
      break;
    }
    case T_UINT32: {
      fprintf(out,"%s  if ( (st->%s%s < %u) || (st->%s%s > %u) ) {\n%s    return %d;\n  %s}\n\n",
        indent,memb->name,arr,memb->type_info.uint32_info.min,
        memb->name,arr,memb->type_info.uint32_info.max,indent,(i + 2) * -1,indent);
      break;
    }
    case T_UINT64: {
      fprintf(out,"%s  if ( (st->%s%s < %" PRIu64 ") || (st->%s%s > %" PRIu64 ") ) {\n%s    return %d;\n  %s}\n\n",
        indent,memb->name,arr,memb->type_info.uint64_info.min,
        memb->name,arr,memb->type_info.uint64_info.max,indent,(i + 2) * -1,indent);
      break;
    }
    case T_FLOAT: {
     fprintf(out,"%s  if ( (st->%s%s < %0.2f) || (st->%s%s > %0.2f) ) {\n%s    return %d;\n  %s}\n\n",
      indent,memb->name,arr,memb->type_info.float_info.min,
      memb->name,arr,memb->type_info.float_info.max,indent,(i + 2) * -1,indent);
     break;
    }
    case T_DOUBLE: {
     fprintf(out,"%s  if ( (st->%s%s < %0.2f) || (st->%s%s > %0.2f) ) {\n%s    return %d;\n  %s}\n\n",
      indent,memb->name,arr,memb->type_info.double_info.min,
      memb->name,arr,memb->type_info.double_info.max,indent,(i + 2) * -1,indent);
     break;
    }
    default : break;
  }

 return;

}

static void codegen_helper_func(struct_data *def, 
  FILE *out, helper_type htype) {

  int i;
  char type[16];
  member_type_info cmp;
  member_info *memb;
  char arr[16];
  char indent[8];
  int isnull = 1;

  switch (htype) {
    case H_INIT: snprintf(type,5,"init"); break;
    case H_RANDOM: snprintf(type,7,"random"); break;
    case H_VALID: snprintf(type,6,"valid"); break;
    default: return;
  }

  memset(&cmp,0,sizeof(member_type_info));

  codegen_helpers_prototype(def,type,out);

  fprintf(out," {\n");

  for (i = 0; i < def->info.member_count; i++) {
    memb = &def->info.members[i];
    if (memcmp(&memb->type_info,&cmp,sizeof(member_type_info))) {
      isnull = 0;
      break;
    }
  }

  for (i = 0; i < def->info.member_count; i++) {
    if ((def->info.members[i].arr || def->info.members[i].len_def[0])) {
      if (htype == H_INIT) {
        if (def->info.members[i].type != T_CHAR) {
          fprintf(out,"  int i;\n\n");
          break;
        }
      } else {
        fprintf(out,"  int i;\n\n");
        break;
      }
    }
  }

  if (htype == H_RANDOM) {
    if (!isnull) {
      if (memb->type != T_STRUCT) {
        fprintf(out,"  struct timeval tv;\n");
        fprintf(out,"  double scale;\n\n");
        fprintf(out,"  gettimeofday(&tv, NULL);\n  srand(tv.tv_sec + tv.tv_usec * 1000000ul);\n\n");
      }
    }
  }

  fprintf(out,"  if (st == NULL) {\n    return -1;\n  }\n\n");

  if (!isnull) {
    if (def->info.is_typedef) {
      if (htype != H_VALID)
        fprintf(out,"  memset(st, 0, sizeof(%s));\n",def->info.name);
    } else {
      if (htype != H_VALID)
        fprintf(out,"  memset(st, 0, sizeof(struct %s));\n",def->info.name);
    }
  }

  if (def->info.type == ST_UNION) {
    fprintf(out,"  switch (idx) {\n");
    codegen_helpers_union_content_from_type(def,type,out);
    fprintf(out,"  }\n\n");
    fprintf(out,"\n  return -1;\n}\n\n");
    return;
  }

  for (i = 0; i < def->info.member_count; i++) {

    memb = &def->info.members[i];

    if (memb->arr || memb->len_def[0]) {
      snprintf(arr,sizeof(arr),"[i]");
      snprintf(indent,sizeof(indent),"  ");
    } else {
      arr[0] = '\0';
      indent[0] = '\0';
    }

    if (htype == H_VALID) {
      if (memb->type == T_CHAR) {
        if (memb->arr && memb->type_info.char_info.notnull) {
          fprintf(out,"  if (!st->%s[0]) {\n    return %d;\n  }\n\n",memb->name,(i + 2) * -1);
        }
      }
    }

    if (memb->arr) {
      if (htype == H_INIT) {
        if (memb->type != T_CHAR) {
          fprintf(out,"  for (i = 0; i < %d; i++) {\n",memb->arr);
        }
      } else {
        fprintf(out,"  for (i = 0; i < %d; i++) {\n",memb->arr);
      }
    }
    else if (memb->len_def[0]) {
      fprintf(out,"  for (i = 0; i < %s; i++) {\n",memb->len_def);
    }

    if (!memb_struct_check(memb)) {
      if (memcmp(&memb->type_info,&cmp,sizeof(member_type_info))) {
        switch(htype) {
          case H_INIT: codegen_helper_init(memb,i,indent,arr,out); break;
          case H_RANDOM: codegen_helper_random(memb,i,indent,arr,out); break;
          case H_VALID: codegen_helper_valid(memb,i,indent,arr,out); break;
        }
      }
    } else {
      if (memb_struct_check(memb) && codegen_is_union(memb->type_info.substruct_info.type_name)) {
        if ((i > 0) && memb_struct_check(&def->info.members[i-1])) {
          fprintf(out,"%s  %s_%s(&st->%s%s,%s_to_index(st->%s));\n",
            indent,memb->type_info.substruct_info.type_name,type,memb->name,arr,
            def->info.members[i-1].type_info.substruct_info.type_name,def->info.members[i-1].name);
        }
      } else if (!codegen_is_enum(memb->type_info.substruct_info.type_name)) {
        fprintf(out,"%s  %s_%s(&st->%s%s);\n",
          indent,memb->type_info.substruct_info.type_name,type,memb->name,arr);
      }
    }

    if ((def->info.members[i].arr || def->info.members[i].len_def[0])) {
      if (htype == H_INIT) {
        if (def->info.members[i].type != T_CHAR) {
          fprintf(out,"  }\n");
        }
      } else {
        fprintf(out,"  }\n");
      }
    }
  }

  fprintf(out,"\n  return 0;\n}\n\n");

}

static void codegen_address_to_patch_func(struct_data *def, 
  FILE *out) {
  int i;
  char upp[256];

  codegen_address_to_patch_prototype(def,out);
  fprintf(out," {\n");
  fprintf(out,"   if (patch == NULL) return NULL;\n  "
    "if (addr->count < 1) return NULL;\n  if (addr->len[0] < 1) return NULL;\n");
  fprintf(out,"  patch->member = %s_strto_member(addr->path[0]);\n  "
    "if (patch->member < 1) return NULL;\n  switch(memb_type) {\n",def->info.name);
  
  for (i = 0; i < def->info.member_count; i++) {
    if (memb_struct_check(&def->info.members[i]) && 
      is_suffix(def->info.members[i].type_info.substruct_info.type_name,"_info") &&
      !codegen_is_union(def->info.members[i].type_info.substruct_info.type_name) && 
      !codegen_is_enum(def->info.members[i].type_info.substruct_info.type_name)) {
      snprintf(upp,sizeof(upp),"%s_%s",def->info.name,def->info.members[i].name);
      uppercase(upp,upp);
      fprintf(out,"      case %s:\n        return %s_address_to_patch(&patch->value.%s_patch,addr);\n",
        upp,def->info.members[i].type_info.substruct_info.type_name,
        def->info.members[i].name);
    }
  }

  fprintf(out,"      default: break;\n    }\n  }\n");
  fprintf(out,"  return patch->value;\n}\n\n");

}

/*static void codegen_strto_patch_func(struct_data *def, 
  FILE *out) {
  int i;
  char *p;
  char name[256];
  char upp[256];

  if (is_prefix(def->info.name,"kr_")) {
    snprintf(name,256,"%s",def->info.name+3);
  } else {
    snprintf(name,256,"%s",def->info.name);
  }
  if (is_suffix(name,"_info")) {
    p = strstr(name,"_info");
    p[0] = '\0';
  }

  codegen_strto_patch_prototype(def,out);
  fprintf(out," {\n");
  fprintf(out,"  int memb_type;\n  int res;\n  char *p;\n  char *pp;\n  char memb_str[%lu];\n\n",
    sizeof(name));
  fprintf(out,"  if (string == NULL || string[0] != '/') return -1;\n");
  fprintf(out,"  if (strncmp(string+1,\"%s\",%zu)) return -1;\n",
    name,strlen(name));
  fprintf(out,"  p = strchr(string+1,'/');\n  if (p == NULL) return 0;"
  " \n  pp = strchr(p+1,'/');\n"
    "  if (pp != NULL) pp[0] = '\\0';\n  res = snprintf(memb_str,%lu,\"%s_%%s\",p+1);\n",
    sizeof(name),def->info.name);
  fprintf(out,"  if (res >= %lu) return -1;\n  memb_type = %s_strto_member(memb_str,res);\n",
    sizeof(name),def->info.name);
  fprintf(out,"  if (memb_type < 0) return -1;\n  patch->member = memb_type;\n"
    "  if (pp != NULL) {\n    pp[0] = '/';\n    switch(memb_type) {\n");

  for (i = 0; i < def->info.member_count; i++) {
    if (memb_struct_check(&def->info.members[i]) && 
      is_suffix(def->info.members[i].type_info.substruct_info.type_name,"_info") &&
      !codegen_is_union(def->info.members[i].type_info.substruct_info.type_name) && 
      !codegen_is_enum(def->info.members[i].type_info.substruct_info.type_name)) {
      snprintf(upp,sizeof(upp),"%s_%s",def->info.name,def->info.members[i].name);
      uppercase(upp,upp);
      fprintf(out,"      case %s:\n        return %s_strto_patch(p,&patch->value.%s_patch);\n",
        upp,def->info.members[i].type_info.substruct_info.type_name,
        def->info.members[i].name);
    }
  }

  fprintf(out,"      default: break;\n    }\n  }\n");

  fprintf(out,"  return 0;\n}\n\n");
}*/

static void codegen_patch_func(struct_data *def, 
  FILE *out) {
  int i;
  codegen_patch_prototype(def,out);
  fprintf(out," {\n");
  if (def->info.member_count) {
    fprintf(out,"  const ptrdiff_t off[%d] = { ",
      def->info.member_count);
    for (i = 0; i < def->info.member_count; i++) {
      if (def->info.is_typedef) {
        fprintf(out,"offsetof(%s, %s)",
          def->info.name,def->info.members[i].name);
      } else {
        fprintf(out,"offsetof(struct %s, %s)",
          def->info.name,def->info.members[i].name);
      }
      if (i != (def->info.member_count - 1)) {
        fprintf(out,", ");
        if (!(i%2))
          fprintf(out,"\n    ");
      }
    }
    fprintf(out,"\n  };\n");
    fprintf(out,"  const size_t sz[%d] = { ",
      def->info.member_count);
    for (i = 0; i < def->info.member_count; i++) {
      if (def->info.is_typedef) {
        fprintf(out,"sizeof(info->%s)",
          def->info.members[i].name);
      } else {
        fprintf(out,"sizeof(info->%s)",
          def->info.members[i].name);
      }
      if (i != (def->info.member_count - 1)) {
        fprintf(out,", ");
        if (!(i%2))
          fprintf(out,"\n    ");
      }
    }
    fprintf(out,"  };\n\n");
  }
  fprintf(out,"  memcpy((char *)info + off[patch->member], &patch->value, sz[patch->member]);\n");
  fprintf(out,"  return 0;\n}\n\n");
}

static void codegen_patch_enum_protos(struct_data *defs, int ndefs, 
  char *prefix, char *suffix, FILE *out) {
  int i;
  int n;
  struct_data *filtered_defs[ndefs];

  for (i = n = 0; i < ndefs; i++) {
    if (is_prefix(defs[i].info.name,prefix) && is_suffix(defs[i].info.name,suffix)) {
      if (defs[i].info.type == ST_STRUCT) {
         filtered_defs[n] = &defs[i];
         n++;
      }
    }
  }

  for (i=0;i<n;i++) {
    fprintf(out,"%s_member %s_strto_member(char *string, int len);\n",
      filtered_defs[i]->info.name,filtered_defs[i]->info.name);
  }
}

static void codegen_enum_protos(struct_data *defs, int ndefs, 
  char *prefix, char *suffix, FILE *out) {
  int i;
  int n;
  struct_data *filtered_defs[ndefs];

  codegen_patch_enum_protos(defs,ndefs,prefix,"_info",out);

  for (i = n = 0; i < ndefs; i++) {
    if (is_prefix(defs[i].info.name,prefix) && is_suffix(defs[i].info.name,suffix)) {
      if (defs[i].info.type == ST_ENUM) {
         filtered_defs[n] = &defs[i];
         n++;
      }
    }
  }

  for (i = 0; i < n; i++) {
    fprintf(out,"int %s_to_index(int val);\n",filtered_defs[i]->info.name);
    fprintf(out,"int kr_strto_%s(char *string);\n",filtered_defs[i]->info.name);
    fprintf(out,"char *kr_strfr_%s(int val);\n",filtered_defs[i]->info.name);
  }

  return;
}

static void codegen_patch_struct(struct_data *def, FILE *out) {
  int i;
  fprintf(out,"typedef struct {\n  int integer;\n  float real;\n");
  for (i = 0; i < def->info.member_count; i++) {
    if (memb_struct_check(&def->info.members[i]) && 
      is_suffix(def->info.members[i].type_info.substruct_info.type_name,"_info") &&
      !codegen_is_union(def->info.members[i].type_info.substruct_info.type_name) && 
      !codegen_is_enum(def->info.members[i].type_info.substruct_info.type_name)) {
      fprintf(out,"  %s_patch %s_patch;\n",
        def->info.members[i].type_info.substruct_info.type_name,
        def->info.members[i].name);
    }
  }
  fprintf(out,"} %s_patch_value;\n\n",def->info.name);
  fprintf(out,"typedef struct {\n  %s_member member;\n  "
    "%s_patch_value value;\n} %s_patch;\n\n",def->info.name,
    def->info.name,def->info.name);
}

static void codegen_member_enum(struct_data *def, FILE *out) {
  int i;
  char struct_name_up[NAME_MAX_LEN];
  char memb_name_up[NAME_MAX_LEN];

  uppercase(def->info.name,struct_name_up);

  fprintf(out,"typedef enum {\n");
  for (i = 0; i < def->info.member_count; i++) {
    uppercase(def->info.members[i].name,memb_name_up);
    fprintf(out,"  %s_%s = %d",struct_name_up,memb_name_up,i+1);
    if (i != (def->info.member_count - 1)) {
      fprintf(out,",");
    }
    fprintf(out,"\n");
  }
  fprintf(out,"} %s_member;\n\n",def->info.name);

}

void codegen_patch_struct_and_member_enum(struct_data *defs, int ndefs, char *prefix,
 char *suffix, FILE *out) {
  int i;
  int n;
  struct_data *filtered_defs[ndefs];

  for (i = n = 0; i < ndefs; i++) {
    if (is_prefix(defs[i].info.name,prefix) 
      && is_suffix(defs[i].info.name,suffix) 
      && defs[i].info.type == ST_STRUCT) {
      filtered_defs[n] = &defs[i];
      n++;
    }
  }

  for (i = 0; i < n; i++) {
    codegen_member_enum(filtered_defs[i],out);
    codegen_patch_struct(filtered_defs[i],out);
  }

}

void codegen_patch_prototypes(struct_data *defs, int ndefs, char *prefix,
 char *suffix, FILE *out) {
  int i;
  int n;
  struct_data *filtered_defs[ndefs];

  for (i = n = 0; i < ndefs; i++) {
    if (is_prefix(defs[i].info.name,prefix) 
      && is_suffix(defs[i].info.name,suffix) 
      && defs[i].info.type == ST_STRUCT) {
      filtered_defs[n] = &defs[i];
      n++;
    }
  }
  for (i = 0; i < n; i++) {
    codegen_patch_prototype(filtered_defs[i],out);
    fprintf(out,";\n");
    codegen_address_to_patch_prototype(filtered_defs[i],out);
    fprintf(out,";\n");
  }
}

void codegen_helpers_prototypes(struct_data *defs, int ndefs, char *prefix,
 char *suffix, FILE *out) {

  int i;
  int n;
  struct_data *filtered_defs[ndefs];

  codegen_patch_prototypes(defs,ndefs,prefix,"_info",out);

  for (i = n = 0; i < ndefs; i++) {
    if (is_prefix(defs[i].info.name,prefix) 
      && is_suffix(defs[i].info.name,suffix) 
      && defs[i].info.type != ST_ENUM) {
      filtered_defs[n] = &defs[i];
      n++;
    }
  }

  for (i = 0; i < n; i++) {
    codegen_helpers_prototype(filtered_defs[i],"init",out);
    fprintf(out,";\n");
    codegen_helpers_prototype(filtered_defs[i],"valid",out);
    fprintf(out,";\n");
    codegen_helpers_prototype(filtered_defs[i],"random",out);
    fprintf(out,";\n");
  }

  codegen_enum_protos(defs,ndefs,prefix,suffix,out);

  return;
}

static void codegen_enum_to_index(struct_data *def, FILE *out) {
  int i;

  for (i = 0; i < def->info.member_count; i++) {
    fprintf(out,"    case %s:\n      return %d;\n",
      def->info.members[i].name,i);
  }

  return;
}

static void codegen_strfr_enum(struct_data *def, FILE *out) {
  int i;

  for (i = 0; i < def->info.member_count; i++) {
    char lowercased[strlen(def->info.members[i].name)+1];
    lowercase(def->info.members[i].name,lowercased);
    fprintf(out,"    case %s:\n      return \"%s\";\n",
      def->info.members[i].name,lowercased);
  }

  return;
}

static void codegen_strto_enum(struct_data *def, FILE *out) {
  int i;

  for (i = 0; i < def->info.member_count; i++) {
    char lowercased[strlen(def->info.members[i].name)+1];
    lowercase(def->info.members[i].name,lowercased);
    fprintf(out,"  if (!strcmp(string,\"%s\")) {\n    return %s;\n  }\n",
      lowercased,def->info.members[i].name);
  }

  return;
}

static void codegen_patch_strto_enum(struct_data *def, FILE *out) {
  int i;
  char upp[NAME_MAX_LEN];

  for (i = 0; i < def->info.member_count; i++) {
    snprintf(upp,NAME_MAX_LEN,"%s_%s",
      def->info.name,def->info.members[i].name);
    uppercase(upp,upp);
    fprintf(out,"  if (!strncmp(string,\"%s\",len)) {\n    return %s;\n  }\n",
      def->info.members[i].name,upp);
  }

  return;
}

static void codegen_patch_enum_util_functions(struct_data *defs, int ndefs, 
  char *prefix, char *suffix, FILE *out) {
  int i;
  int n;
  struct_data *filtered_defs[ndefs];

  for (i = n = 0; i < ndefs; i++) {
    if (is_prefix(defs[i].info.name,prefix) && is_suffix(defs[i].info.name,suffix)) {
      if (defs[i].info.type == ST_STRUCT) {
         filtered_defs[n] = &defs[i];
         n++;
      }
    }
  }

  for (i=0;i<n;i++) {
    fprintf(out,"%s_member %s_strto_member(char *string, int len) {\n",
      filtered_defs[i]->info.name,filtered_defs[i]->info.name);
    codegen_patch_strto_enum(filtered_defs[i],out);
    fprintf(out,"  return -1;\n}\n\n");
  }
}

int codegen_enum_util_functions(struct_data *defs, int ndefs, 
  char *prefix, char *suffix, FILE *out) {

  int i;
  int n;
  struct_data *filtered_defs[ndefs];

  codegen_patch_enum_util_functions(defs,ndefs,prefix,"_info",out);

  for (i = n = 0; i < ndefs; i++) {
    if (is_prefix(defs[i].info.name,prefix) && is_suffix(defs[i].info.name,suffix)) {
      if (defs[i].info.type == ST_ENUM) {
         filtered_defs[n] = &defs[i];
         n++;
      }
    }
  }

  for (i=0;i<n;i++) {
    fprintf(out,"int %s_to_index(int val) {\n  switch (val) {\n",filtered_defs[i]->info.name);
    codegen_enum_to_index(filtered_defs[i],out);
    fprintf(out,"  }\n  return -1;\n}\n\n");

    fprintf(out,"char *kr_strfr_%s(int val) {\n  switch (val) {\n",filtered_defs[i]->info.name);
    codegen_strfr_enum(filtered_defs[i],out);
    fprintf(out,"  }\n  return NULL;\n}\n\n");
    
    fprintf(out,"int kr_strto_%s(char *string) {\n",filtered_defs[i]->info.name);
    codegen_strto_enum(filtered_defs[i],out);
    fprintf(out,"  return -1;\n}\n\n");

  }

  return 0;
}

static int codegen_patch_functions(struct_data *defs, int ndefs, char *prefix,
 char *suffix, FILE *out) {

  int i;
  int n;
  struct_data *filtered_defs[ndefs];

  for (i = n = 0; i < ndefs; i++) {
    if (is_prefix(defs[i].info.name,prefix) 
      && is_suffix(defs[i].info.name,suffix) 
      && defs[i].info.type == ST_STRUCT) {
      filtered_defs[n] = &defs[i];
      n++;
    }
  }

  for (i = 0; i < n; i++) {
    codegen_patch_func(filtered_defs[i],out);
    codegen_address_to_patch_func(filtered_defs[i],out);
  }

  return 0;
}

int codegen_helper_functions(struct_data *defs, int ndefs, char *prefix,
 char *suffix, FILE *out) {

  int i;
  int n;
  struct_data *filtered_defs[ndefs];

  codegen_patch_functions(defs,ndefs,prefix,"_info",out);

  for (i = n = 0; i < ndefs; i++) {
    if (is_prefix(defs[i].info.name,prefix) 
      && is_suffix(defs[i].info.name,suffix) 
      && defs[i].info.type != ST_ENUM) {
      filtered_defs[n] = &defs[i];
      n++;
    }
  }

  for (i = 0; i < n; i++) {
    codegen_helper_func(filtered_defs[i],out,H_INIT);
    codegen_helper_func(filtered_defs[i],out,H_VALID);
    codegen_helper_func(filtered_defs[i],out,H_RANDOM);
  }

  return 0;

}