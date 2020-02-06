/**
 * Connect to server &
 * Get the temperature value every 2 sec
 * 
 * @author Lee, Sangheon
 * @version open62541 1.1v
 * @see None
 */

#include <open62541/client_config_default.h>
#include <open62541/client_highlevel.h>
#include <open62541/plugin/log_stdout.h>

#include <stdlib.h>

/* Execute python program for influxdb query
 * @param : temperature */
static void influxdb(int temperature){
	char cmd[] = "python ./influxdb_write.py ";
	char final_cmd[256];
	char value[128];
	int execute_res;
	
	/* Convert temperature value int to char[] */
	sprintf(value, "%d", temperature);
	
	/* final_cmd = cmd + temperature */
	snprintf(final_cmd, sizeof final_cmd, "%s%s", cmd, value);
	
	/* Execute the final_cmd */
	execute_res = system(final_cmd);
	if ((execute_res) == 0)
	{
		// success to call execve()
	}
	else if(execute_res == 127)
	{
		// fail to call execve()
	}
	else if(execute_res == -1)
	{
		// occur other error
	}
	else
	{
		// command's return code
	}
}

UA_Boolean running = true;
static void stopHandler(int sign) {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_CLIENT, "Received Ctrl-C");
    running = 0;
}

int main(void) {
	signal(SIGINT, stopHandler); /* catches ctrl-c */
	
    UA_Client *client = UA_Client_new();
    UA_ClientConfig_setDefault(UA_Client_getConfig(client));
    UA_StatusCode retval = UA_Client_connect(client, "opc.tcp://localhost:4840");
    if(retval != UA_STATUSCODE_GOOD) {
        UA_Client_delete(client);
        return (int)retval;
    }

    /* Read the value attribute of the node. UA_Client_readValueAttribute is a
     * wrapper for the raw read service available as UA_Client_Service_read. */
    UA_Variant value; /* Variants can hold scalar values and arrays of any type */
    UA_Variant_init(&value);

    /* NodeId of the variable holding the current time */
    const UA_NodeId nodeId = UA_NODEID_STRING(1, "temperature");

	/* Read & Get the temperature value from the temperature node every 2 sec
	 * Save it in influxDB */
	while(running){
		retval = UA_Client_readValueAttribute(client, nodeId, &value);
		if(retval == UA_STATUSCODE_GOOD && UA_Variant_hasScalarType(&value, &UA_TYPES[UA_TYPES_INT32])) {
			UA_Int32 temperature = *(UA_Int32 *) value.data;
			UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Temperature : %d\n", temperature);
			influxdb(temperature);
		}
		UA_sleep_ms(2000);
	}
	
    /* Clean up */
    UA_Variant_clear(&value);
    UA_Client_delete(client); /* Disconnects the client internally */
    return EXIT_SUCCESS;
}