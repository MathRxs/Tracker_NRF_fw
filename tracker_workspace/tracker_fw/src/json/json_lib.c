#include "json_lib.h"

/**
 * This function adds a new object to the parent object.
 * 
 * @param parent The parent object to add the new object to.
 * @param str The name of the object
 * @param item the cJSON object to add to the parent
 * 
 * @return The return value is the number of bytes written, which is the size of the buffer.
 */
int json_add_obj(cJSON *parent, const char *str, cJSON *item)
{
	cJSON_AddItemToObject(parent, str, item);

	return 0;
}

/**
 * It creates a JSON object of type string and adds it to the parent object
 * 
 * @param parent The parent object to add the new object to.
 * @param str The name of the item to add to the parent object.
 * @param item The name of the item to add to the parent object.
 */
int json_add_str(cJSON *parent, const char *str, const char *item)
{
	cJSON *json_str;

	json_str = cJSON_CreateString(item);
	if (json_str == NULL) {
		return -ENOMEM;
	}

	return json_add_obj(parent, str, json_str);
}

/**
 * It creates a JSON object of type number and adds it to the parent object.
 * 
 * @param parent The parent object to add the new object to.
 * @param str The name of the object.
 * @param item The item to add to the JSON object.
 */
int json_add_number(cJSON *parent, const char *str, double item)
{
	cJSON *json_num;

	json_num = cJSON_CreateNumber(item);
	if (json_num == NULL) {
		return -ENOMEM;
	}

	return json_add_obj(parent, str, json_num);
}
