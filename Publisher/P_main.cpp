#include <cstdlib>
#include <iostream>
#include <time.h>
#include <unistd.h>

#include "Publisher.h"

using namespace std;


int main(int argc, char** argv)
{
	try {
		// The default Face will connect using a Unix socket, or to "localhost".
		Face face;

		// Use the system default key chain and certificate name to sign commands.
		KeyChain keyChain;
		face.setCommandSigningInfo(keyChain, keyChain.getDefaultCertificateName());

		// Also use the default certificate name to sign data packets.
		Publisher publisher(keyChain, keyChain.getDefaultCertificateName());
		Name prefix("/vide1");
		cout << "Register prefix  " << prefix.toUri() << endl;
		// TODO: After we remove the registerPrefix with the deprecated OnInterest,
		// we can remove the explicit cast to OnInterestCallback (needed for boost).
		face.registerPrefix(prefix, (const OnInterestCallback&)func_lib::ref(publisher), func_lib::ref(publisher));
		

		publisher.start();
		// The main event loop.
		// Wait forever to receive one interest for the prefix.
		while ( publisher.stat ) {
			face.processEvents();
			// We need to sleep for a few milliseconds so we don't use 100% of the CPU.
			usleep(10000);
		}
	}
	catch (std::exception& e) {
		cout << "exception: " << e.what() << endl;
	}
	return 0;
}