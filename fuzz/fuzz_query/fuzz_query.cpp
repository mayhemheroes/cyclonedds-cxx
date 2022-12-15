/*
 * Copyright(c) 2006 to 2021 ZettaScale Technology and others
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License v. 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0, or the Eclipse Distribution License
 * v. 1.0 which is available at
 * http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * SPDX-License-Identifier: EPL-2.0 OR BSD-3-Clause
 */
#include <fuzzer/FuzzedDataProvider.h>

#include "dds/dds.hpp"
//#include <gtest/gtest.h>
#include "Space.hpp"


/**
 * Fixture for the DataReader tests
 */

/**
class Query : public ::testing::Test
{
public:
    dds::domain::DomainParticipant participant;
    dds::sub::Subscriber subscriber;
    dds::topic::Topic<Space::Type1> topic;
    dds::sub::DataReader<Space::Type1> reader;

    std::vector<std::string> params;
    std::string expression;


    Query() :
        participant(dds::core::null),
        subscriber(dds::core::null),
        topic(dds::core::null),
        reader(dds::core::null),
        params(),
        expression("long_1 = %0 and long_2 = %1 and long_3 = %2")
    {
        const char *paramsinit[] = {"1", "2", "3"};
        this->params = std::vector<std::string>(paramsinit, paramsinit+3);
    }

    void SetUp()
    {
        this->participant = dds::domain::DomainParticipant(org::eclipse::cyclonedds::domain::default_id());
        //ASSERT_NE(this->participant, dds::core::null);

        this->subscriber = dds::sub::Subscriber(this->participant);
        //ASSERT_NE(this->subscriber, dds::core::null);

        this->topic = dds::topic::Topic<Space::Type1>(this->participant, "query_fuzz_topic");
        //ASSERT_NE(this->topic, dds::core::null);

        this->reader = dds::sub::DataReader<Space::Type1>(this->subscriber, this->topic);
        //ASSERT_NE(this->reader, dds::core::null);
    }

    void check(const dds::sub::Query &query,
               const std::string &expected_expression,
               const std::vector<std::string> &expected_params)
    {
        std::vector<std::string> plist;
        dds::sub::Query::const_iterator it;
        //ASSERT_EQ(query.parameters_length(), expected_params.size());
        for (it = query.begin(); it != query.end(); ++it) {
            plist.push_back(*it);
        }
        //ASSERT_EQ(plist, expected_params);
        //ASSERT_EQ(query.data_reader(), this->reader);
        //ASSERT_EQ(query.expression(), expected_expression);
    }

    void TearDown()
    {
        this->reader = dds::core::null;
        this->topic = dds::core::null;
        this->subscriber = dds::core::null;
        this->participant = dds::core::null;
    }

};

**/

/**
 * Tests
 */

/**
TEST_F(Query, null)
{
    dds::sub::Query query1 = dds::core::null;
    dds::sub::Query query2(dds::core::null);
    ASSERT_EQ(query1, dds::core::null);
    ASSERT_EQ(query2, dds::core::null);
}

TEST_F(Query, create_reader_null)
{
    ASSERT_THROW({
        dds::sub::Query query(dds::core::null, this->expression, this->params);
    }, dds::core::NullReferenceError);
}

TEST_F(Query, create_params_none)
{
    dds::sub::Query query(this->reader,
                          this->expression);
    this->check(query, this->expression, std::vector<std::string>());
}

TEST_F(Query, create_params_vector)
{
    dds::sub::Query query(this->reader,
                          this->expression,
                          this->params);
    this->check(query, this->expression, this->params);
}

TEST_F(Query, create_params_iter)
{
    dds::sub::Query query(this->reader,
                          this->expression,
                          this->params.begin(),
                          this->params.end());
    this->check(query, this->expression, this->params);
}
**/

/**
TEST_F(Query, expression)
{
    dds::sub::Query query(this->reader,
                          this->expression);
    std::string new_expr("my test expression");
    query.expression(new_expr);
    this->check(query, new_expr, std::vector<std::string>());
}
**/

/**
TEST_F(Query, parameters)
{
    std::vector<std::string> plist;
    dds::sub::Query query(this->reader,
                          this->expression);

    const char *p1[] = {"1", "2", "3"};
    std::vector<std::string> params1(p1, p1+3);
    plist.insert(plist.end(), params1.begin(), params1.end());
    query.parameters(params1.begin(), params1.end());
    this->check(query, this->expression, plist);

    const char *p2[] = {"42", "22"};
    std::vector<std::string> params2(p2, p2+2);
    plist.insert(plist.end(), params2.begin(), params2.end());
    query.parameters(params2.begin(), params2.end());
    this->check(query, this->expression, plist);
}
**/

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
  // Set up
  dds::domain::DomainParticipant participant = dds::domain::DomainParticipant(org::eclipse::cyclonedds::domain::default_id());
  dds::sub::Subscriber subscriber = dds::sub::Subscriber(participant);
  dds::topic::Topic<Space::Type1> topic = dds::topic::Topic<Space::Type1>(participant, "query_fuzz_topic");;
  dds::sub::DataReader<Space::Type1> reader = dds::sub::DataReader<Space::Type1>(subscriber, topic);
  std::string expression = "long_1 = %0 and long_2 = %1 and long_3 = %2";
  const char *params_init[] = {"1", "2", "3"};
  std::vector<std::string> params = std::vector<std::string>(params_init, params_init+3);
  dds::sub::Query query(reader, expression);

  // Prep fuzz data
  FuzzedDataProvider provider(data, size);

  switch(data[0] % 2)
  {
    //TEST_F(Query, expression)
    case 0:
    {
      std::string new_expr = provider.ConsumeRemainingBytesAsString();
      query.expression(new_expr);
      break;
    }
    ///TEST_F(Query, parameters)
    case 1:
    {
      std::vector<std::string> new_params;
      size_t size_tracker = size;
      std::string fuzzed_str;
      while(size_tracker >= 0) {
        //string_size = rand() % (size - 1) + 1;
        fuzzed_str = provider.ConsumeRandomLengthString(size);
        new_params.push_back(fuzzed_str);
        size_tracker -= fuzzed_str.size();
        //current_size -= string_size;
      }
      query.parameters(new_params.begin(), new_params.end());
      break;
    }
  }

  // Tear down
  reader = dds::core::null;
  topic = dds::core::null;
  subscriber = dds::core::null;
  participant = dds::core::null;

  return 0;
}
