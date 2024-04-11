#include <stdio.h>
#include <json.h>

int log_info(const char *message)
{
    struct json_object *jobj;

    jobj = json_object_new_object();

    json_object_object_add(jobj, "timestamp_", json_object_new_string("This is timestamp"));
    json_object_object_add(jobj, "message", json_object_new_string(message));
    json_object_object_add(jobj, "log.level", json_object_new_string("INFO"));

    printf("%s\n", json_object_to_json_string_ext(jobj, JSON_C_TO_STRING_PLAIN));

    json_object_put(jobj);

    return 0;
}

int log_error(const char *message)
{
    struct json_object *jobj;

    jobj = json_object_new_object();

    json_object_object_add(jobj, "timestamp_", json_object_new_string("This is timestamp"));
    json_object_object_add(jobj, "message", json_object_new_string(message));
    json_object_object_add(jobj, "log.level", json_object_new_string("ERROR"));

    printf("%s\n", json_object_to_json_string_ext(jobj, JSON_C_TO_STRING_PLAIN));

    json_object_put(jobj);

    return 0;
}
