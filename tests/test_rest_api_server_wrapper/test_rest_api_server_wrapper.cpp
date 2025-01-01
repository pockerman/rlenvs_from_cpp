#include "rlenvs/envs/api_server/apiserver.h"
#include "rlenvs/rlenvs_types_v2.h"

#include <gtest/gtest.h>
#include <unordered_map>
#include <string>
#include <any>

namespace{

using namespace rlenvscpp;
using rlenvscpp::envs::RESTApiServerWrapper;
const std::string SERVER_URL = "http://0.0.0.0:8001/api";

}

TEST(TestRESTApiServerWrapper, Constructor) {

    RESTApiServerWrapper api_server(SERVER_URL);
    ASSERT_EQ(api_server.get_url(), SERVER_URL);
}


TEST(TestRESTApiServerWrapper, register_new) {

    RESTApiServerWrapper api_server(SERVER_URL);
    
	api_server.register_new("Dummy", "/dummy-env-uri");
	ASSERT_EQ(api_server.get_uri("Dummy"), "/dummy-env-uri");
}

TEST(TestRESTApiServerWrapper, register_new_throws) {

    RESTApiServerWrapper api_server(SERVER_URL);
    
	const std::string name("Dummy");
	api_server.register_new(name, "/dummy-env-uri");
	
	try {
        api_server.register_new(name, "/another/dummy-env-uri");
    }
    catch(const std::logic_error& err) {
        EXPECT_EQ(err.what(), std::string("Environment: " + name + " already exists"));
    }
    catch(...) {
        FAIL() << "Expected std::std::logic_error";
    }
	
}
