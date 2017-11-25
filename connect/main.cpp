#include <iostream>
#include <memory>

#include <boost/smart_ptr/make_unique.hpp>
#include <boost/smart_ptr/make_shared.hpp>

#include <cassandra.h>

CassError connect_session(CassSession * session, CassCluster * cluster) 
{
	CassError rc = CASS_OK;
	CassFuture * future = cass_session_connect(session, cluster);

	cass_future_wait(future);
	rc = cass_future_error_code(future);
	cass_future_free(future);

	return rc;
}

CassError execute_query(CassSession * session, const std::string & query) 
{
	CassError rc = CASS_OK;
	CassFuture * future = NULL;
	CassStatement * statement = cass_statement_new(query.c_str(), 0);

	future = cass_session_execute(session, statement);
	cass_future_wait(future);

	rc = cass_future_error_code(future);
	if (rc != CASS_OK) 
	{
		print_error(future);
	}

	cass_future_free(future);
	cass_statement_free(statement);
	return rc;
}

int main(int argc, char** argv) 
{
	int retVal = 1;
	try 
	{
		std::string hosts("127.0.0.1");

		CassCluster * cluster = cass_cluster_new();
		cass_cluster_set_contact_points(cluster, hosts.c_str());
		CassSession * session = cass_session_new();
		CassFuture * close_future;

		if (connect_session(session, cluster) != CASS_OK) 
		{
			cass_cluster_free(cluster);
			cass_session_free(session);
			throw std::exception("cannot connect");
		}

		retVal = 0;
	}
	catch (std::exception & e) 
	{
		std::cout << e.what() << std::endl;
	}

	// wait for a return
	std::cout << "Press any key..." << std::endl;
	std::cin.get();
	return retVal;
}
