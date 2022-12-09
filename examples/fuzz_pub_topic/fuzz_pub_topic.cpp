/*
 * Copyright(c) 2006 to 2020 ZettaScale Technology and others
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License v. 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0, or the Eclipse Distribution License
 * v. 1.0 which is available at
 * http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * SPDX-License-Identifier: EPL-2.0 OR BSD-3-Clause
 */
#include <cstdlib>
#include <iostream>
#include <chrono>
#include <thread>

/* Include the C++ DDS API. */
#include "dds/dds.hpp"

/* Include data type and specific traits to be used with the C++ DDS API. */
#include "FuzzData.hpp"

using namespace org::eclipse::cyclonedds;
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {

        // child process because return value zero
        if (fork() == 0){
        // std::cout << "=== [Subscriber] Create reader." << std::endl;

        /* First, a domain participant is needed.
         * Create one on the default domain. */
        dds::domain::DomainParticipant participant(domain::default_id());

        /* To subscribe to something, a topic is needed. */
        dds::topic::Topic<FuzzData::Msg> topic(participant, "FuzzData_Msg");

        /* A reader also needs a subscriber. */
        dds::sub::Subscriber subscriber(participant);

        /* Now, the reader can be created to subscribe to a HelloWorld message. */
        dds::sub::DataReader<FuzzData::Msg> reader(subscriber, topic);

        /* Poll until a message has been read.
         * It isn't really recommended to do this kind wait in a polling loop.
         * It's done here just to illustrate the easiest way to get data.
         * Please take a look at Listeners and WaitSets for much better
         * solutions, albeit somewhat more elaborate ones. */
        // std::cout << "=== [Subscriber] Wait for message." << std::endl;
        bool poll = true;
        while (poll) {
            /* For this example, the reader will return a set of messages (aka
             * Samples). There are other ways of getting samples from reader.
             * See the various read() and take() functions that are present. */
            dds::sub::LoanedSamples<FuzzData::Msg> samples;

            /* Try taking samples from the reader. */
            samples = reader.take();

            /* Are samples read? */
            if (samples.length() > 0) {
                /* Use an iterator to run over the set of samples. */
                dds::sub::LoanedSamples<FuzzData::Msg>::const_iterator sample_iter;
                for (sample_iter = samples.begin();
                     sample_iter < samples.end();
                     ++sample_iter) {
                    /* Get the message and sample information. */
                    const FuzzData::Msg& msg = sample_iter->data();
                    const dds::sub::SampleInfo& info = sample_iter->info();

                    /* Sometimes a sample is read, only to indicate a data
                     * state change (which can be found in the info). If
                     * that's the case, only the key value of the sample
                     * is set. The other data parts are not.
                     * Check if this sample has valid data. */
                    if (info.valid()) {
                        // std::cout << "=== [Subscriber] Message received:" << std::endl;
                        // std::cout << "    userID  : " << msg.userID() << std::endl;
                        // std::cout << "    Message : \"" << msg.message() << "\"" << std::endl;

                        /* Only 1 message is expected in this example. */
                        poll = false;
                    }
                }
            } else {
                std::this_thread::sleep_for(std::chrono::milliseconds(20));
            }
        }
        }
        // parent process because return value non-zero.
        else{
        // std::cout << "=== [Publisher] Create writer." << std::endl;

        /* First, a domain participant is needed.
         * Create one on the default domain. */
        dds::domain::DomainParticipant participant(domain::default_id());

        /* To publish something, a topic is needed. */
        //dds::topic::Topic<FuzzData::Msg> topic(participant, str);
        dds::topic::Topic<FuzzData::Msg> topic(participant, "FuzzData_Msg");

        /* A writer also needs a publisher. */
        dds::pub::Publisher publisher(participant);

        /* Now, the writer can be created to publish a HelloWorld message. */
        dds::pub::DataWriter<FuzzData::Msg> writer(publisher, topic);

        /* For this example, we'd like to have a subscriber to actually read
         * our message. This is not always necessary. Also, the way it is
         * done here is just to illustrate the easiest way to do so. It isn't
         * really recommended to do a wait in a polling loop, however.
         * Please take a look at Listeners and WaitSets for much better
         * solutions, albeit somewhat more elaborate ones. */
        // std::cout << "=== [Publisher] Waiting for subscriber." << std::endl;
        // while (writer.publication_matched_status().current_count() == 0) {
        //     std::this_thread::sleep_for(std::chrono::milliseconds(20));
        // }

        /* Create a message to write. */

        std::string str(reinterpret_cast<const char*>(data), size);

        FuzzData::Msg msg(1, str);

        /* Write the message. */
        // std::cout << "=== [Publisher] Write sample." << std::endl;
        writer.write(msg);

        /* With a normal configuration (see dds::pub::qos::DataWriterQos
         * for various different writer configurations), deleting a writer will
         * dispose all its related message.
         * Wait for the subscriber to have stopped to be sure it received the
         * message. Again, not normally necessary and not recommended to do
         * this in a polling loop. */
        // std::cout << "=== [Publisher] Waiting for sample to be accepted." << std::endl;
        while (writer.publication_matched_status().current_count() > 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    
        // std::cout << "=== [Publisher] Done." << std::endl;
        }
           
    return EXIT_SUCCESS;
}
