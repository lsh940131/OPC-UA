/**
 * Create variable node &
 * Update temperature value every 2 sec
 * 
 * @author Lee, Sangheon
 * @version open62541 1.1v
 * @see None
 */
  
#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include <open62541/server_config_default.h>

#include <signal.h>
#include <stdlib.h>
#include <time.h>

/* Gernerate random number
 * @param : range is you want random numbers bounds (0 ~ range-1)
 * @return: random number based on time */
static UA_Int32
generateRandomValue(UA_Int32 range) {
	srand(time(NULL));
	UA_Int32 random = (UA_Int32) rand() % range;

	return random;
}

/* Update temperature value with random number
 * @param : UA_Server */
static void
updateTemperaure(UA_Server *server) {
    UA_Int32 temperature = generateRandomValue(10);
	UA_Variant value;
    UA_Variant_setScalar(&value, &temperature, &UA_TYPES[UA_TYPES_INT32]);
    UA_NodeId currentNodeId = UA_NODEID_STRING(1, "temperature");
    UA_Server_writeValue(server, currentNodeId, value);
	UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Temperature = %d", temperature);
}

/* Add variable node for save temperature value
 * @param : UA_Server */
static void
addVariable(UA_Server *server) {
    /* Define the attribute of the temperature variable node */
    UA_Int32 temperature = 2;
	UA_VariableAttributes attr = UA_VariableAttributes_default;
    attr.displayName = UA_LOCALIZEDTEXT("en-US","temperature");
    attr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
	UA_Variant_setScalar(&attr.value, &temperature, &UA_TYPES[UA_TYPES_INT32]);

    /* Add the variable node to the information model */
    UA_NodeId currentNodeId = UA_NODEID_STRING(1, "temperature");
    UA_QualifiedName currentName = UA_QUALIFIEDNAME(1, "temperature");
    UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
    UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
    UA_NodeId variableTypeNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE);
    UA_Server_addVariableNode(server, currentNodeId, parentNodeId,
                              parentReferenceNodeId, currentName,
                              variableTypeNodeId, attr, NULL, NULL);
	
	updateTemperaure(server);
}

/* Callback Function
 * Update temperature value */
static void
Callback(UA_Server *server, void *data) {
	updateTemperaure(server);
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Update Temperature");
}

UA_Boolean running = true;
static void stopHandler(int sign) {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "received ctrl-c");
    running = false;
}

int main(void) {
    signal(SIGINT, stopHandler);
    signal(SIGTERM, stopHandler);

    UA_Server *server = UA_Server_new();
    UA_ServerConfig_setDefault(UA_Server_getConfig(server));
	
	addVariable(server);

    /* Add a repeated callback to the server */
    UA_Server_addRepeatedCallback(server, Callback, NULL, 2000, NULL); /* call every 2 sec */

    UA_StatusCode retval = UA_Server_run(server, &running);

    UA_Server_delete(server);
    return retval == UA_STATUSCODE_GOOD ? EXIT_SUCCESS : EXIT_FAILURE;
}