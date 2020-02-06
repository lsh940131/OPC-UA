/**
 * Create object, variables
 * 
 * @author Lee, Sangheon
 * @version open62541 1.1v
 * @see None
 */

#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include <open62541/server_config_default.h>

#include <signal.h>

static void createNodes(UA_Server *server) {
	// Create Object node
    UA_NodeId nodeId; /* get the nodeid assigned by the server */
    UA_ObjectAttributes oAttr = UA_ObjectAttributes_default;
    oAttr.displayName = UA_LOCALIZEDTEXT("en-US", "Variable Collections");
    UA_Server_addObjectNode(server, UA_NODEID_NULL,
                            UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
                            UA_QUALIFIEDNAME(1, "Variable Collections"), UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE),
                            oAttr, NULL, &nodeId);

	// Create Variable node (Int32) in Object Node
    UA_VariableAttributes int32Attr = UA_VariableAttributes_default;
    UA_Int32 int32_data = 334;
    UA_Variant_setScalar(&int32Attr.value, &int32_data, &UA_TYPES[UA_TYPES_INT32]);
    int32Attr.displayName = UA_LOCALIZEDTEXT("en-US", "My Variable 1");
    UA_Server_addVariableNode(server, UA_NODEID_NULL, nodeId,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                              UA_QUALIFIEDNAME(1, "My Variable 1"),
                              UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), int32Attr, NULL, NULL);

	// Create Variable node (Int64) in Object Node
    UA_VariableAttributes int64Attr = UA_VariableAttributes_default;
    UA_Int64 int64_data = 12321;
    UA_Variant_setScalar(&int64Attr.value, &int64_data, &UA_TYPES[UA_TYPES_INT64]);
    int64Attr.displayName = UA_LOCALIZEDTEXT("en-US", "My Variable 2");
    UA_Server_addVariableNode(server, UA_NODEID_NULL, nodeId,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                              UA_QUALIFIEDNAME(1, "My Variable 2"),
                              UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), int64Attr, NULL, NULL);

	// Create Variable node (String) in Object Node
    UA_VariableAttributes stringAttr = UA_VariableAttributes_default;
    UA_String string_data = UA_STRING("Temporary string");
    UA_Variant_setScalar(&stringAttr.value, &string_data, &UA_TYPES[UA_TYPES_STRING]);
    stringAttr.displayName = UA_LOCALIZEDTEXT("en-US", "My Variable 3");
    UA_Server_addVariableNode(server, UA_NODEID_NULL, nodeId,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                              UA_QUALIFIEDNAME(1, "My Variable 3"),
                              UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), stringAttr, NULL, NULL);

	// Create Variable node (Int32 Array) in Object Node
    UA_VariableAttributes int32ArrayAttr = UA_VariableAttributes_default;
    UA_Int32 int32_array[10] = {10, 11, 23, 54, 55, 1353, 434, 65, 6, 9};
	UA_Variant_setArray(&int32ArrayAttr.value, int32_array, 10, &UA_TYPES[UA_TYPES_INT32]);
    int32ArrayAttr.displayName = UA_LOCALIZEDTEXT("en-US", "My Array variable 1");
    UA_Server_addVariableNode(server, UA_NODEID_NULL, nodeId,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                              UA_QUALIFIEDNAME(1, "My Array variable 1"),
                              UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), int32ArrayAttr, NULL, NULL);
}

// Set up Stop function
static volatile UA_Boolean running = true;
static void stopHandler(int sign) {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "received ctrl-c");
    running = false;
}

int main(void) {
	// If receive interrupt or terminate, End the server.
    signal(SIGINT, stopHandler);
    signal(SIGTERM, stopHandler);

	// Initialize server
    UA_Server *server = UA_Server_new();
    UA_ServerConfig_setDefault(UA_Server_getConfig(server));

	// Create nodes
    createNodes(server);

	// Run the server
    UA_StatusCode retval = UA_Server_run(server, &running);

	// End the server
    UA_Server_delete(server);
	
    return retval == UA_STATUSCODE_GOOD ? EXIT_SUCCESS : EXIT_FAILURE;
}
