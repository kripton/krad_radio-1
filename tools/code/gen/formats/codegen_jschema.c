#include "codegen_utils.h"

static char *memb_type_to_json_type(member_info *memb) {

  char *type;

  type = member_type_to_str(memb->type);

  if (!strcmp(type,"int") || !strncmp(type,"int32_t",7)) {
    return "number";
  }

  if (!strcmp(type,"uint") || !strncmp(type,"uint32_t",8)) {
    return "number";
  }

  if (!strncmp(type,"int64_t",7)) {
    return "number";
  }

  if (!strncmp(type,"uint64_t",8)) {
    return "number";
  }

  if (!strncmp(type,"float",5)) {
    return "number";
  }

  if (!strncmp(type,"char",4)) {
    if (memb->arr) {
      return "string";
    } else  if (memb->ptr) {
        if (memb->ptr == 1) {
          return "string";
        } else {
          return NULL;
        }
    } else {
      return "string";
    }
  }

  return NULL;
}

static void codegen_jschema_memb_limits(member_info *memb, FILE *out) {

  member_type type;
  member_type_info *info;

  type = memb->type;
  info = &memb->type_info;

  switch (type) {
    case T_CHAR: {
      if (memb->arr) {
        fprintf(out,"\"maxLength\" : %d, ",memb->arr - 1);
        if (info->char_info.notnull) {
          fprintf(out,"\"minLength\" : 1, ");
        }
      }
      break;
    }
    case T_INT32: {
      fprintf(out,"\"minimum\" : %d, ",info->int32_info.min);
      fprintf(out,"\"maximum\" : %d, ",info->int32_info.max);
      break;
    }
    case T_INT64: {
      fprintf(out,"\"minimum\" : %" PRId64 ", ",info->int64_info.min);
      fprintf(out,"\"maximum\" : %" PRId64 ", ",info->int64_info.max);
      break;
    }
    case T_UINT32: {
      fprintf(out,"\"minimum\" : %u, ",info->uint32_info.min);
      fprintf(out,"\"maximum\" : %u, ",info->uint32_info.max);
      break;
    }
    case T_UINT64: {
      fprintf(out,"\"minimum\" : %" PRIu64 ", ",info->uint64_info.min);
      fprintf(out,"\"maximum\" : %" PRIu64 ", ",info->uint64_info.max);
      break;
    }
    case T_FLOAT: {
     fprintf(out,"\"minimum\" : %0.2f, ",info->float_info.min);
     fprintf(out,"\"maximum\" : %0.2f, ",info->float_info.max);
     break;
   }
   case T_DOUBLE: {
     fprintf(out,"\"minimum\" : %0.2f, ",info->double_info.min);
     fprintf(out,"\"maximum\" : %0.2f, ",info->double_info.max);
     break;
   }
   default : break;
 }
  return;
}

static void codegen_jschema_enum(struct_data *def, FILE *out) {
  int i;

  fprintf(out,"\"enum\": [");

  for (i = 0; i < def->info.member_count; i++) {
    fprintf(out,"\"%s\"",def->info.members[i].name);
    if (i != (def->info.member_count - 1)) {
      fprintf(out,",");
    }
  }

  fprintf(out,"]");

  return;
}

static void codegen_jschema_internal(struct_data *def, struct_data **defs, FILE *out) {

  int j;
  int idx;
  int actual_memb_count;
  member_info *actual_members[def->info.member_count];
  member_type_info cmp;
  const char *type;

  memset(&cmp,0,sizeof(member_type_info));

  for (j = actual_memb_count = 0; j < def->info.member_count; j++) {
    if (memb_type_to_json_type(&def->info.members[j])) {
      actual_members[actual_memb_count] = &def->info.members[j];
      actual_memb_count++;
    } else if ((idx = memb_struct_check(&def->info.members[j]))) {
      if (defs[idx-1] && (defs[idx-1]->info.type != ST_UNION)) {
        actual_members[actual_memb_count] = &def->info.members[j];
        actual_memb_count++;
      }
    }
  }

  fprintf(out,"\"type\" : \"object\",");
  fprintf(out,"\"properties\" : {");

  for (j = 0; j < actual_memb_count; j++) {
    if ( (type = memb_type_to_json_type(actual_members[j])) ) {

      fprintf(out,"\"%s\" : {",actual_members[j]->name);
      fprintf(out,"\"type\" : \"%s\", ",type);
      if (memcmp(&actual_members[j]->type_info,&cmp,sizeof(member_type_info))) {
        codegen_jschema_memb_limits(actual_members[j],out);
      }
      fprintf(out,"\"required\" : true ");
      fprintf(out,"}");

      if (j != (actual_memb_count - 1)) {
        fprintf(out,",");
      }

    } else if ( (idx = memb_struct_check(actual_members[j])) ) {

      if (defs[idx-1] && (defs[idx-1]->info.type == ST_STRUCT)) {
        fprintf(out,"\"%s\" : {",actual_members[j]->name);
        codegen_jschema_internal(defs[idx-1],defs,out);
        fprintf(out,"}");

        if (j != (actual_memb_count - 1)) {
          fprintf(out,",");
        }
      } else if (defs[idx-1] && (defs[idx-1]->info.type == ST_ENUM)) {

        fprintf(out,"\"%s\" : {",actual_members[j]->name);
        codegen_jschema_enum(defs[idx-1],out);
        fprintf(out,"}");

        if (j != (actual_memb_count - 1)) {
          fprintf(out,",");
        }
      }
    } 
  }

  fprintf(out,"}");

  return;
}

int codegen_jschema(struct_data **defs, int ndefs, 
  char *prefix, char *suffix, FILE *out) {

  int i;
  int n;
  struct_data *filtered_defs[ndefs];

  for (i = n = 0; i < ndefs; i++) {
    if (defs[i] && is_prefix(defs[i]->info.name,prefix) && is_suffix(defs[i]->info.name,suffix) && defs[i]->info.type == ST_STRUCT) {
      filtered_defs[n] = defs[i];
      n++;
    }
  }

  if (n) {
    fprintf(out,"[\n");
  }

  for (i = 0; i < n; i++) {
    fprintf(out,"{");
    fprintf(out,"\"name\" : \"%s\",",filtered_defs[i]->info.name);
    codegen_jschema_internal(filtered_defs[i],defs,out);
    fprintf(out,"}");
    if (i != (n - 1)) {
      fprintf(out,",");
    }
    fprintf(out,"\n");
  }

  if (n) {
    fprintf(out,"]\n");
  }
  
  return 0;
}