//
// JSONTest.cpp
//
// $Id: //poco/1.4/XML/testsuite/src/JSONTest.cpp#1 $
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#include "JSONTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"

#include "Poco/JSON/Object.h"
#include "Poco/JSON/Parser.h"
#include "Poco/JSON/Query.h"
#include "Poco/JSON/JSONException.h"
#include "Poco/JSON/Stringifier.h"
#include "Poco/JSON/ParseHandler.h"
#include "Poco/JSON/PrintHandler.h"
#include "Poco/JSON/Template.h"

#include "Poco/Path.h"
#include "Poco/Environment.h"
#include "Poco/File.h"
#include "Poco/FileStream.h"
#include "Poco/Glob.h"
#include "Poco/UTF8Encoding.h"
#include "Poco/Latin1Encoding.h"
#include "Poco/TextConverter.h"

#include <set>

using namespace Poco::JSON;
using namespace Poco::Dynamic;


JSONTest::JSONTest(const std::string& name): CppUnit::TestCase("JSON")
{

}


JSONTest::~JSONTest()
{

}


void JSONTest::setUp()
{
}


void JSONTest::tearDown()
{
}


void JSONTest::testNullProperty()
{
	std::string json = "{ \"test\" : null }";
	Parser parser;

	Var result;
	try
	{
		result = parser.parse(json);
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assert(false);
	}
	assert(result.type() == typeid(Object::Ptr));

	Object::Ptr object = result.extract<Object::Ptr>();
	assert(object->isNull("test"));
	Var test = object->get("test");
	assert(test.isEmpty());
}


void JSONTest::testTrueProperty()
{
	std::string json = "{ \"test\" : true }";
	Parser parser;
	Var result;

	try
	{
		result = parser.parse(json);
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assert(false);
	}

	assert(result.type() == typeid(Object::Ptr));

	Object::Ptr object = result.extract<Object::Ptr>();
	Var test = object->get("test");
	assert(test.type() == typeid(bool));
	bool value = test;
	assert(value);
}


void JSONTest::testFalseProperty()
{
	std::string json = "{ \"test\" : false }";
	Parser parser;
	Var result;

	try
	{
		result = parser.parse(json);
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assert(false);
	}

	assert(result.type() == typeid(Object::Ptr));

	Object::Ptr object = result.extract<Object::Ptr>();
	Var test = object->get("test");
	assert(test.type() == typeid(bool));
	bool value = test;
	assert(!value);
}


void JSONTest::testNumberProperty()
{
	std::string json = "{ \"test\" : 1969 }";
	Parser parser;
	Var result;

	try
	{
		result = parser.parse(json);
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assert(false);
	}

	assert(result.type() == typeid(Object::Ptr));

	Object::Ptr object = result.extract<Object::Ptr>();
	Var test = object->get("test");
	assert(test.isInteger());
	int value = test;
	assert(value == 1969);
}


void JSONTest::testUnsignedNumberProperty()
{
	// 4294967295 == unsigned(-1)
	std::string json = "{ \"test\" : 4294967295 }";
	Parser parser;
	Var result;

	try
	{
		result = parser.parse(json);
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assert(false);
	}

	assert(result.type() == typeid(Object::Ptr));

	Object::Ptr object = result.extract<Object::Ptr>();
	Var test = object->get("test");
	assert(test.isInteger());
	unsigned value = test;
	assert(value == -1);
}

#if defined(POCO_HAVE_INT64)


void JSONTest::testNumber64Property()
{
	std::string json = "{ \"test\" : -5000000000000000 }";
	Parser parser;
	Var result;

	try
	{
		result = parser.parse(json);
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assert(false);
	}

	assert(result.type() == typeid(Object::Ptr));

	Object::Ptr object = result.extract<Object::Ptr>();
	Var test = object->get("test");
	assert(test.isInteger());
	Poco::Int64 value = test;
	assert(value == -5000000000000000);
}


void JSONTest::testUnsignedNumber64Property()
{
	// 18446744073709551615 == UInt64(-1)
	std::string json = "{ \"test\" : 18446744073709551615 }";
	Parser parser;
	Var result;

	try
	{
		result = parser.parse(json);
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assert(false);
	}

	assert(result.type() == typeid(Object::Ptr));

	Object::Ptr object = result.extract<Object::Ptr>();
	Var test = object->get("test");
	assert(test.isInteger());
	Poco::UInt64 value = test;
	assert(value == -1);
}

#endif


void JSONTest::testStringProperty()
{
	std::string json = "{ \"test\" : \"value\" }";
	Parser parser;
	Var result;

	try
	{
		result = parser.parse(json);
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assert(false);
	}

	assert(result.type() == typeid(Object::Ptr));

	Object::Ptr object = result.extract<Object::Ptr>();
	Var test = object->get("test");
	assert(test.isString());
	std::string value = test.convert<std::string>();
	assert(value.compare("value") == 0);
}


void JSONTest::testEmptyObject()
{
	std::string json = "{}";
	Parser parser;
	Var result;

	try
	{
		result = parser.parse(json);
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assert(false);
	}

	assert(result.type() == typeid(Object::Ptr));

	Object::Ptr object = result.extract<Object::Ptr>();
	assert(object->size() == 0);
}


void JSONTest::testComplexObject()
{
	std::string json = 
	"{"
		"\"id\": 1,"
		"\"jsonrpc\": \"2.0\","
		"\"total\": 1,"
		"\"result\": ["
			"{"
				"\"id\": 1,"
				"\"guid\": \"67acfb26-17eb-4a75-bdbd-f0669b7d8f73\","
				"\"picture\": \"http://placehold.it/32x32\","
				"\"age\": 40,"
				"\"name\": \"Angelina Crossman\","
				"\"gender\": \"female\","
				"\"company\": \"Raylog\","
				"\"phone\": \"892-470-2803\","
				"\"email\": \"angelina@raylog.com\","
				"\"address\": \"20726, CostaMesa, Horatio Streets\","
				"\"about\": \"Consectetuer suscipit volutpat eros dolor euismod, "
				"et dignissim in feugiat sed, ea tation exerci quis. Consectetuer, "
				"dolor dolore ad vero ullamcorper, tincidunt facilisis at in facilisi, "
				"iusto illum illum. Autem nibh, sed elit consequat volutpat tation, "
				"nisl lorem lorem sed tation, facilisis dolore. Augue odio molestie, "
				"dolor zzril nostrud aliquam sed, wisi dolor et ut iusto, ea. Magna "
				"ex qui facilisi, hendrerit quis in eros ut, zzril nibh dolore nisl "
				"suscipit, vulputate elit ut lobortis exerci, nulla dolore eros at "
				"aliquam, ullamcorper vero ad iusto. Adipiscing, nisl eros exerci "
				"nisl vel, erat in luptatum in duis, iusto.\","
				"\"registered\": \"2008-04-09T11:13:17 +05:00\","
				"\"tags\": ["
					"\"ut\","
					"\"accumsan\","
					"\"feugait\","
					"\"ex\","
					"\"odio\","
					"\"consequat\","
					"\"delenit\""
				"],"
				"\"friends\": ["
					"{"
						"\"id\": 1,"
						"\"name\": \"Hailey Hardman\""
					"},"
					"{"
						"\"id\": 2,"
						"\"name\": \"Bailey Oldridge\""
					"},"
					"{"
						"\"id\": 3,"
						"\"name\": \"Makayla Campbell\""
					"}"
				"]"
			"}"
		"]"
	"}";
	
	Parser parser;
	Var result;

	try
	{
		result = parser.parse(json);
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assert(false);
	}

	assert(result.type() == typeid(Object::Ptr));
}


void JSONTest::testDoubleProperty()
{
	std::string json = "{ \"test\" : 123.45 }";
	Parser parser;
	Var result;

	try
	{
		result = parser.parse(json);
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assert(false);
	}

	assert(result.type() == typeid(Object::Ptr));

	Object::Ptr object = result.extract<Object::Ptr>();
	Var test = object->get("test");
	assert(test.isNumeric());
	double value = test;
	assert(value == 123.45);
}


void JSONTest::testDouble2Property()
{
	std::string json = "{ \"test\" : 12e34 }";
	Parser parser;
	Var result;

	try
	{
		result = parser.parse(json);
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assert(false);
	}

	assert(result.type() == typeid(Object::Ptr));

	Object::Ptr object = result.extract<Object::Ptr>();
	Var test = object->get("test");
	assert(test.isNumeric());
	double value = test;
	assert(value >= 1.99e34 && value <= 12.01e34);
}


void JSONTest::testDouble3Property()
{
	std::string json = "{ \"test\" : 12e-34 }";
	Parser parser;
	Var result;

	try
	{
		result = parser.parse(json);
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assert(false);
	}

	assert(result.type() == typeid(Object::Ptr));

	Object::Ptr object = result.extract<Object::Ptr>();
	Var test = object->get("test");
	assert(test.isNumeric());
	double value = test;
	assert(value == 12e-34);
}


void JSONTest::testObjectProperty()
{
	std::string json = "{ \"test\" : { \"property\" : \"value\" } }";
	Parser parser;
	Var result;

	try
	{
		result = parser.parse(json);
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assert(false);
	}

	assert(result.type() == typeid(Object::Ptr));
	
	Object::Ptr object = result.extract<Object::Ptr>();
	assert (object->isObject("test"));
	assert (!object->isArray("test"));

	Var test = object->get("test");
	assert(test.type() == typeid(Object::Ptr));
	object = test.extract<Object::Ptr>();

	test = object->get("property");
	assert(test.isString());
	std::string value = test.convert<std::string>();
	assert(value.compare("value") == 0);
}


void JSONTest::testObjectArray()
{
	std::string json = "{ \"test\" : { \"test1\" : [1, 2, 3], \"test2\" : 4 } }";
	Parser parser;
	Var result;

	try
	{
		result = parser.parse(json);
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assert(false);
	}

	assert(result.type() == typeid(Object::Ptr));
	Object::Ptr object = result.extract<Object::Ptr>();
	assert(object->isObject("test"));
	object = object->getObject("test");
	assert(!object->isObject("test1"));
	assert(object->isArray("test1"));
	assert(!object->isObject("test2"));
	assert(!object->isArray("test2"));
}


void JSONTest::testArrayOfObjects()
{
	std::string json = "[ {\"test\" : 0}, { \"test1\" : [1, 2, 3], \"test2\" : 4 } ]";
	Parser parser;
	Var result;

	try
	{
		result = parser.parse(json);
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assert(false);
	}

	assert(result.type() == typeid(Array::Ptr));
	Array::Ptr arr = result.extract<Array::Ptr>();
	Object::Ptr object = arr->getObject(0);
	assert (object->getValue<int>("test") == 0);
	object = arr->getObject(1);
	arr = object->getArray("test1");
	result = arr->get(0);
	assert (result == 1);

}


void JSONTest::testEmptyArray()
{
	std::string json = "[]";
	Parser parser;
	Var result;

	try
	{
		result = parser.parse(json);
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assert(false);
	}

	assert(result.type() == typeid(Array::Ptr));

	Array::Ptr array = result.extract<Array::Ptr>();
	assert(array->size() == 0);
}


void JSONTest::testNestedArray()
{
	std::string json = "[[[[]]]]";
	Parser parser;
	Var result;

	try
	{
		result = parser.parse(json);
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assert(false);
	}

	assert(result.type() == typeid(Array::Ptr));

	Array::Ptr array = result.extract<Array::Ptr>();
	assert(array->size() == 1);
}


void JSONTest::testNullElement()
{
	std::string json = "[ null ]";
	Parser parser;
	Var result;

	try
	{
		result = parser.parse(json);
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assert(false);
	}

	assert(result.type() == typeid(Array::Ptr));

	Array::Ptr array = result.extract<Array::Ptr>();
	assert(array->isNull(0));
	Var test = array->get(0);
	assert(test.isEmpty());
}


void JSONTest::testTrueElement()
{
	std::string json = "[ true ]";
	Parser parser;
	Var result;

	try
	{
		result = parser.parse(json);
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assert(false);
	}

	assert(result.type() == typeid(Array::Ptr));

	Array::Ptr array = result.extract<Array::Ptr>();
	Var test = array->get(0);
	assert(test.type() == typeid(bool));
	bool value = test;
	assert(value);
}


void JSONTest::testFalseElement()
{
	std::string json = "[ false ]";
	Parser parser;
	Var result;

	try
	{
		result = parser.parse(json);
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assert(false);
	}

	assert(result.type() == typeid(Array::Ptr));

	Array::Ptr array = result.extract<Array::Ptr>();
	Var test = array->get(0);
	assert(test.type() == typeid(bool));
	bool value = test;
	assert(!value);
}


void JSONTest::testNumberElement()
{
	std::string json = "[ 1969 ]";
	Parser parser;
	Var result;

	try
	{
		result = parser.parse(json);
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assert(false);
	}

	assert(result.type() == typeid(Array::Ptr));

	Array::Ptr array = result.extract<Array::Ptr>();
	Var test = array->get(0);
	assert(test.isInteger());
	int value = test;
	assert(value == 1969);
}


void JSONTest::testStringElement()
{
	std::string json = "[ \"value\" ]";
	Parser parser;
	Var result;

	try
	{
		result = parser.parse(json);
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assert(false);
	}

	assert(result.type() == typeid(Array::Ptr));

	Array::Ptr array = result.extract<Array::Ptr>();
	Var test = array->get(0);
	assert(test.isString());
	std::string value = test.convert<std::string>();
	assert(value.compare("value") == 0);
}


void JSONTest::testEmptyObjectElement()
{
	std::string json = "[{}]";
	Parser parser;
	Var result;

	try
	{
		result = parser.parse(json);
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assert(false);
	}

	assert(result.type() == typeid(Array::Ptr));

	Array::Ptr array = result.extract<Array::Ptr>();
	Object::Ptr object = array->getObject(0);
	assert(object->size() == 0);
}


void JSONTest::testDoubleElement()
{
	std::string json = "[ 123.45 ]";
	Parser parser;
	Var result;

	try
	{
		result = parser.parse(json);
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assert(false);
	}

	assert(result.type() == typeid(Array::Ptr));

	Array::Ptr array = result.extract<Array::Ptr>();
	Var test = array->get(0);
	assert(test.isNumeric());
	double value = test;
	assert(value == 123.45);
}


void JSONTest::testOptValue()
{
	std::string json = "{ }";
	Parser parser;
	Var result;

	try
	{
		result = parser.parse(json);
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assert(false);
	}

	assert(result.type() == typeid(Object::Ptr));

	Object::Ptr object = result.extract<Object::Ptr>();
	int n = object->optValue("test", 123);
	assert(n == 123);
}


void JSONTest::testQuery()
{
	std::string json = "{ \"name\" : \"Franky\", \"children\" : [ \"Jonas\", \"Ellen\" ] }";
	Parser parser;
	Var result;

	try
	{
		result = parser.parse(json);
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assert(false);
	}

	assert(result.type() == typeid(Object::Ptr));

	Query query(result);

	std::string firstChild = query.findValue("children[0]", "");
	assert(firstChild.compare("Jonas") == 0);
}


void JSONTest::testPrintHandler()
{
	std::string json = "{ \"name\" : \"Homer\", \"age\" : 38, \"wife\" : \"Marge\", \"age\" : 36, \"children\" : [ \"Bart\", \"Lisa\", \"Maggie\" ] }";
	Parser parser;
	std::ostringstream ostr;
	PrintHandler::Ptr pHandler = new PrintHandler(ostr);
	parser.setHandler(pHandler);
	parser.parse(json);
	assert (ostr.str() == "{\"name\":\"Homer\",\"age\":38,\"wife\":\"Marge\",\"age\":36,\"children\":[\"Bart\",\"Lisa\",\"Maggie\"]}");

	pHandler->setIndent(1);
	ostr.str("");
	parser.reset();
	parser.parse(json);
	assert (ostr.str() == "{\n"
		" \"name\" : \"Homer\",\n"
		" \"age\" : 38,\n"
		" \"wife\" : \"Marge\",\n"
		" \"age\" : 36,\n"
		" \"children\" : [\n"
		"  \"Bart\",\n"
		"  \"Lisa\",\n"
		"  \"Maggie\"\n"
		" ]\n"
		"}"
	);

	pHandler->setIndent(2);
	ostr.str("");
	parser.reset();
	parser.parse(json);
	assert (ostr.str() == "{\n"
		"  \"name\" : \"Homer\",\n"
		"  \"age\" : 38,\n"
		"  \"wife\" : \"Marge\",\n"
		"  \"age\" : 36,\n"
		"  \"children\" : [\n"
		"    \"Bart\",\n"
		"    \"Lisa\",\n"
		"    \"Maggie\"\n"
		"  ]\n"
		"}"
	);

	pHandler->setIndent(4);
	ostr.str("");
	parser.reset();
	parser.parse(json);
	assert (ostr.str() == "{\n"
		"    \"name\" : \"Homer\",\n"
		"    \"age\" : 38,\n"
		"    \"wife\" : \"Marge\",\n"
		"    \"age\" : 36,\n"
		"    \"children\" : [\n"
		"        \"Bart\",\n"
		"        \"Lisa\",\n"
		"        \"Maggie\"\n"
		"    ]\n"
		"}"
	);
}


void JSONTest::testStringify()
{
	std::string json = "{ \"Simpsons\" : { \"husband\" : { \"name\" : \"Homer\" , \"age\" : 38 }, \"wife\" : { \"name\" : \"Marge\", \"age\" : 36 }, "
						"\"children\" : [ \"Bart\", \"Lisa\", \"Maggie\" ], "
						"\"address\" : { \"number\" : 742, \"street\" : \"Evergreen Terrace\", \"town\" : \"Springfield\" } } }";
	Parser parser;
	Var result;

	try
	{
		result = parser.parse(json);
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assert(false);
	}

	assert(result.type() == typeid(Object::Ptr));
	std::ostringstream ostr;

	Stringifier::condense(result, ostr);
	std::string str = "{"
						"\"Simpsons\":{"
						"\"address\":{"
						"\"number\":742,"
						"\"street\":\"Evergreen Terrace\","
						"\"town\":\"Springfield\""
						"},"
						"\"children\":["
						"\"Bart\","
						"\"Lisa\","
						"\"Maggie\"],"
						"\"husband\":{"
						"\"age\":38,"
						"\"name\":\"Homer\""
						"},"
						"\"wife\":{"
						"\"age\":36,\"name\":\"Marge\""
						"}}}";
	assert (ostr.str() == str);

	ostr.str("");
	Stringifier::stringify(result, ostr);
	assert (ostr.str() == str);

	ostr.str("");
	Stringifier::stringify(result, ostr, 1);
	str = "{\n"
		" \"Simpsons\" : {\n"
		"  \"address\" : {\n"
		"   \"number\" : 742,\n"
		"   \"street\" : \"Evergreen Terrace\",\n"
		"   \"town\" : \"Springfield\"\n"
		"  },\n"
		"  \"children\" : [\n"
		"   \"Bart\",\n"
		"   \"Lisa\",\n"
		"   \"Maggie\"\n"
		"  ],\n"
		"  \"husband\" : {\n"
		"   \"age\" : 38,\n"
		"   \"name\" : \"Homer\"\n"
		"  },\n"
		"  \"wife\" : {\n"
		"   \"age\" : 36,\n"
		"   \"name\" : \"Marge\"\n"
		"  }\n"
		" }\n"
		"}";
	assert (ostr.str() == str);

	ostr.str("");
	Stringifier::stringify(result, ostr, 2);
	str = "{\n"
		"  \"Simpsons\" : {\n"
		"    \"address\" : {\n"
		"      \"number\" : 742,\n"
		"      \"street\" : \"Evergreen Terrace\",\n"
		"      \"town\" : \"Springfield\"\n"
		"    },\n"
		"    \"children\" : [\n"
		"      \"Bart\",\n"
		"      \"Lisa\",\n"
		"      \"Maggie\"\n"
		"    ],\n"
		"    \"husband\" : {\n"
		"      \"age\" : 38,\n"
		"      \"name\" : \"Homer\"\n"
		"    },\n"
		"    \"wife\" : {\n"
		"      \"age\" : 36,\n"
		"      \"name\" : \"Marge\"\n"
		"    }\n"
		"  }\n"
		"}";
	assert (ostr.str() == str);

	ostr.str("");
	Stringifier::stringify(result, ostr, 4);
	str = "{\n"
		"    \"Simpsons\" : {\n"
		"        \"address\" : {\n"
		"            \"number\" : 742,\n"
		"            \"street\" : \"Evergreen Terrace\",\n"
		"            \"town\" : \"Springfield\"\n"
		"        },\n"
		"        \"children\" : [\n"
		"            \"Bart\",\n"
		"            \"Lisa\",\n"
		"            \"Maggie\"\n"
		"        ],\n"
		"        \"husband\" : {\n"
		"            \"age\" : 38,\n"
		"            \"name\" : \"Homer\"\n"
		"        },\n"
		"        \"wife\" : {\n"
		"            \"age\" : 36,\n"
		"            \"name\" : \"Marge\"\n"
		"        }\n"
		"    }\n"
		"}";
	assert (ostr.str() == str);
}


void JSONTest::testStringifyPreserveOrder()
{
	std::string json = "{ \"Simpsons\" : { \"husband\" : { \"name\" : \"Homer\" , \"age\" : 38 }, \"wife\" : { \"name\" : \"Marge\", \"age\" : 36 }, "
						"\"children\" : [ \"Bart\", \"Lisa\", \"Maggie\" ], "
						"\"address\" : { \"number\" : 742, \"street\" : \"Evergreen Terrace\", \"town\" : \"Springfield\" } } }";

	ParseHandler::Ptr pHandler = new ParseHandler(true);
	Parser parser(pHandler);
	Var result;

	try
	{
		result = parser.parse(json);
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assert(false);
	}

	assert(result.type() == typeid(Object::Ptr));
	std::ostringstream ostr;

	Stringifier::condense(result, ostr);
	std::cout << ostr.str() << std::endl;
	assert (ostr.str() == "{\"Simpsons\":{\"husband\":{\"name\":\"Homer\",\"age\":38},\"wife\":{\"name\":\"Marge\",\"age\":36},"
						"\"children\":[\"Bart\",\"Lisa\",\"Maggie\"],"
						"\"address\":{\"number\":742,\"street\":\"Evergreen Terrace\",\"town\":\"Springfield\"}}}");

	ostr.str("");
	Stringifier::stringify(result, ostr);
	assert (ostr.str() == "{\"Simpsons\":{\"husband\":{\"name\":\"Homer\",\"age\":38},\"wife\":{\"name\":\"Marge\",\"age\":36},"
						"\"children\":[\"Bart\",\"Lisa\",\"Maggie\"],"
						"\"address\":{\"number\":742,\"street\":\"Evergreen Terrace\",\"town\":\"Springfield\"}}}");
	
	ostr.str("");
	Stringifier::stringify(result, ostr, 1);
	assert (ostr.str() == "{\n"
						" \"Simpsons\" : {\n"
						"  \"husband\" : {\n"
						"   \"name\" : \"Homer\",\n"
						"   \"age\" : 38\n"
						"  },\n"
						"  \"wife\" : {\n"
						"   \"name\" : \"Marge\",\n"
						"   \"age\" : 36\n"
						"  },\n"
						"  \"children\" : [\n"
						"   \"Bart\",\n"
						"   \"Lisa\",\n"
						"   \"Maggie\"\n"
						"  ],\n"
						"  \"address\" : {\n"
						"   \"number\" : 742,\n"
						"   \"street\" : \"Evergreen Terrace\",\n" 
						"   \"town\" : \"Springfield\"\n"
						"  }\n"
						" }\n"
						"}");

	ostr.str("");
	Stringifier::stringify(result, ostr, 2);
	assert (ostr.str() == "{\n"
						"  \"Simpsons\" : {\n"
						"    \"husband\" : {\n"
						"      \"name\" : \"Homer\",\n"
						"      \"age\" : 38\n"
						"    },\n"
						"    \"wife\" : {\n"
						"      \"name\" : \"Marge\",\n"
						"      \"age\" : 36\n"
						"    },\n"
						"    \"children\" : [\n"
						"      \"Bart\",\n"
						"      \"Lisa\",\n"
						"      \"Maggie\"\n"
						"    ],\n"
						"    \"address\" : {\n"
						"      \"number\" : 742,\n"
						"      \"street\" : \"Evergreen Terrace\",\n" 
						"      \"town\" : \"Springfield\"\n"
						"    }\n"
						"  }\n"
						"}");

	ostr.str("");
	Stringifier::stringify(result, ostr, 4);
	assert (ostr.str() == "{\n"
						"    \"Simpsons\" : {\n"
						"        \"husband\" : {\n"
						"            \"name\" : \"Homer\",\n"
						"            \"age\" : 38\n"
						"        },\n"
						"        \"wife\" : {\n"
						"            \"name\" : \"Marge\",\n"
						"            \"age\" : 36\n"
						"        },\n"
						"        \"children\" : [\n"
						"            \"Bart\",\n"
						"            \"Lisa\",\n"
						"            \"Maggie\"\n"
						"        ],\n"
						"        \"address\" : {\n"
						"            \"number\" : 742,\n"
						"            \"street\" : \"Evergreen Terrace\",\n" 
						"            \"town\" : \"Springfield\"\n"
						"        }\n"
						"    }\n"
						"}");
}


void JSONTest::testValidJanssonFiles()
{
	Poco::Path pathPattern(getTestFilesPath("valid"));

	std::set<std::string> paths;
	Poco::Glob::glob(pathPattern, paths);

	for(std::set<std::string>::iterator it = paths.begin(); it != paths.end(); ++it)
	{
		Poco::Path filePath(*it, "input");

		if ( filePath.isFile() )
		{
			Poco::File inputFile(filePath);
			if ( inputFile.exists() )
			{
				Poco::FileInputStream fis(filePath.toString());
				std::cout << filePath.toString() << std::endl;

				Parser parser;
				Var result;

				try
				{
					parser.parse(fis);
					result = parser.result();
					std::cout << "Ok!" << std::endl;
				}
				catch(JSONException& jsone)
				{
					std::string err = jsone.displayText();
					std::cout << "Failed:" << err << std::endl;
					fail (err);
				}
				catch(Poco::Exception& e)
				{
					std::string err = e.displayText();
					std::cout << "Failed:" << err << std::endl;
					fail (err);
				}
			}
		}
	}
}


void JSONTest::testInvalidJanssonFiles()
{
	Poco::Path pathPattern(getTestFilesPath("invalid"));

	std::set<std::string> paths;
	Poco::Glob::glob(pathPattern, paths);

	for(std::set<std::string>::iterator it = paths.begin(); it != paths.end(); ++it)
	{
		Poco::Path filePath(*it, "input");

		if ( filePath.isFile() )
		{
			Poco::File inputFile(filePath);
			if ( inputFile.exists() )
			{
				Poco::FileInputStream fis(filePath.toString());
				std::cout << filePath.toString() << std::endl;

				Parser parser;
				parser.setAllowNullByte(false);
				Var result;

				try
				{
					parser.parse(fis);
					result = parser.result();
					// We shouldn't get here.
					std::cout << "We didn't get an exception. This is the result: " << result.convert<std::string>() << std::endl; 
					fail(result.convert<std::string>());
				}
				catch(JSONException&)
				{
					continue;
				}
				catch(Poco::SyntaxException&)
				{ }
			}
		}
	}
}


void JSONTest::testInvalidUnicodeJanssonFiles()
{
	Poco::Path pathPattern(getTestFilesPath("invalid-unicode"));

	std::set<std::string> paths;
	Poco::Glob::glob(pathPattern, paths);

	for(std::set<std::string>::iterator it = paths.begin(); it != paths.end(); ++it)
	{
		Poco::Path filePath(*it, "input");

		if ( filePath.isFile() )
		{
			Poco::File inputFile(filePath);
			if ( inputFile.exists() )
			{
				Poco::FileInputStream fis(filePath.toString());
				std::cout << filePath.toString() << std::endl;

				Parser parser;
				parser.setAllowNullByte(false);
				Var result;

				try
				{
					parser.parse(fis);
					result = parser.result();
					// We shouldn't get here.
					std::cout << "We didn't get an exception. This is the result: " << result.convert<std::string>() << std::endl; 
					fail(result.convert<std::string>());
				}
				catch(JSONException&)
				{
					continue;
				}
				catch(Poco::SyntaxException&)
				{ }
			}
		}
	}
}


void JSONTest::testTemplate()
{
	Template tpl;
	tpl.parse("Hello world! From <?= person.name?>.\n<?if person.tooOld?>You're too old.<?endif?>");

	Object::Ptr data = new Object();
	Object::Ptr person = new Object();
	data->set("person", person);
	person->set("name", "Franky");
	person->set("tooOld", true);
	std::ostringstream ostr;
	tpl.render(data, ostr);
	std::cout << ostr.str();
	assert (ostr.str() == "Hello world! From Franky.\nYou're too old.");
}


void JSONTest::testUnicode()
{
	const unsigned char supp[] = {0x61, 0xE1, 0xE9, 0x78, 0xED, 0xF3, 0xFA, 0x0};
	std::string text((const char*) supp);

	std::string json = "{ \"test\" : \"a\\u00E1\\u00E9x\\u00ED\\u00F3\\u00FA\" }";
	Parser parser;

	Var result;
	try
	{
		parser.parse(json);
		result = parser.result();
	}
	catch(JSONException& jsone)
	{
		std::cout << jsone.message() << std::endl;
		assert(false);
	}
	assert(result.type() == typeid(Object::Ptr));

	Object::Ptr object = result.extract<Object::Ptr>();
	Var test = object->get("test");

	Poco::Latin1Encoding latin1;
	Poco::UTF8Encoding utf8;
	Poco::TextConverter converter(latin1, utf8);
	std::string original;
	converter.convert(text, original);

	assert(test.convert<std::string>() == original);
}


std::string JSONTest::getTestFilesPath(const std::string& type)
{
	std::ostringstream ostr;
	ostr << "data/" << type << '/';
	std::string validDir(ostr.str());
	Poco::Path pathPattern(validDir);
	if (Poco::File(pathPattern).exists())
	{
		validDir += '*';
		return validDir;
	}

	ostr.str("");
	ostr << "/JSON/testsuite/data/" << type << '/';
	validDir = Poco::Environment::get("POCO_BASE") + ostr.str();
	pathPattern = validDir;

	if (Poco::File(pathPattern).exists())
		validDir += '*';
	else
	{
		std::cout << "Can't find " << validDir << std::endl;
		throw Poco::NotFoundException("cannot locate directory containing valid JSON test files");
	}
	return validDir;
}


CppUnit::Test* JSONTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("JSONTest");

	CppUnit_addTest(pSuite, JSONTest, testNullProperty);
	CppUnit_addTest(pSuite, JSONTest, testTrueProperty);
	CppUnit_addTest(pSuite, JSONTest, testFalseProperty);
	CppUnit_addTest(pSuite, JSONTest, testNumberProperty);
	CppUnit_addTest(pSuite, JSONTest, testUnsignedNumberProperty);
#if defined(POCO_HAVE_INT64)
	CppUnit_addTest(pSuite, JSONTest, testNumber64Property);
	CppUnit_addTest(pSuite, JSONTest, testUnsignedNumber64Property);
#endif
	CppUnit_addTest(pSuite, JSONTest, testStringProperty);
	CppUnit_addTest(pSuite, JSONTest, testEmptyObject);
	CppUnit_addTest(pSuite, JSONTest, testComplexObject);
	CppUnit_addTest(pSuite, JSONTest, testDoubleProperty);
	CppUnit_addTest(pSuite, JSONTest, testDouble2Property);
	CppUnit_addTest(pSuite, JSONTest, testDouble3Property);
	CppUnit_addTest(pSuite, JSONTest, testObjectProperty);
	CppUnit_addTest(pSuite, JSONTest, testObjectArray);
	CppUnit_addTest(pSuite, JSONTest, testArrayOfObjects);
	CppUnit_addTest(pSuite, JSONTest, testEmptyArray);
	CppUnit_addTest(pSuite, JSONTest, testNestedArray);
	CppUnit_addTest(pSuite, JSONTest, testNullElement);
	CppUnit_addTest(pSuite, JSONTest, testTrueElement);
	CppUnit_addTest(pSuite, JSONTest, testFalseElement);
	CppUnit_addTest(pSuite, JSONTest, testNumberElement);
	CppUnit_addTest(pSuite, JSONTest, testStringElement);
	CppUnit_addTest(pSuite, JSONTest, testEmptyObjectElement);
	CppUnit_addTest(pSuite, JSONTest, testDoubleElement);
	CppUnit_addTest(pSuite, JSONTest, testOptValue);
	CppUnit_addTest(pSuite, JSONTest, testQuery);
	CppUnit_addTest(pSuite, JSONTest, testPrintHandler);
	CppUnit_addTest(pSuite, JSONTest, testStringify);
	CppUnit_addTest(pSuite, JSONTest, testStringifyPreserveOrder);
	CppUnit_addTest(pSuite, JSONTest, testValidJanssonFiles);
	CppUnit_addTest(pSuite, JSONTest, testInvalidJanssonFiles);
	CppUnit_addTest(pSuite, JSONTest, testInvalidUnicodeJanssonFiles);
	CppUnit_addTest(pSuite, JSONTest, testTemplate);
	CppUnit_addTest(pSuite, JSONTest, testUnicode);

	return pSuite;
}