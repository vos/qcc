# Qt CryptoChat (QCC)

Qt CryptoChat (QCC) is a free instant messenger for Windows, Linux and Mac OS X. QCC uses a custom network protocol for handling the transfer of all network packets between the dedicated QCC-server and the connected QCC-clients. All messages between the server and the clients are encrypted using the well known [RSA public-key encryption algorithm](http://en.wikipedia.org/wiki/RSA) with a strong session key.  QCC is build on the [Qt framework](http://qt.nokia.com) from Nokia to provide cross-platform support.


## Installation

If you want to build QCC from the [source files](http://github.com/vos/qcc), you will need to make several preparations.

### Dependencies

QCC requires the following external libraries:

 * [Qt](http://qt.nokia.com) 4.7 or greater
 * [Qt Cryptographic Architecture](http://delta.affinix.com/qca/) (QCA) 2.0 or greater
 * [QCA OpenSSL provider plugin](http://delta.affinix.com/qca/) 2.0 or greater
    * [OpenSSL](http://www.openssl.org) 0.9.6 or greater

Install all the libraries above (there may be some precompiled binary files available for your platform, if you don't want to compile all the packages by yourself you might want to check that first).

### Compilation

If you have installed all the required libraries from above, the easiest way to compile QCC itself is to download the complete [Qt SDK](http://qt.nokia.com/downloads) (if you haven't already done so). Then you can start up the provided Qt Creator IDE and open the QCC project file **qcc.pro** (the one inside the root folder of QCC) and hit the **Build All** button.  Alternatively you can use `qmake` on the command line if you prefer (see the [Qt documentation](http://doc.qt.nokia.com/latest/qmake-manual.html) for further information).


## Usage

### Server

The QCC-server has no graphical user interface (GUI) and no user input. The server uses a classical console output mainly for debug purposes and for server administrators to see what's happening on the QCC-server.  You can change the port on which the server will listen for incoming client connections with the `-port` command line argument, e.g.

    $ ./qcc-server -port 12345

The default port is set to **54321** for testing purposes.

### Client

The QCC-client uses a simple to use GUI which should be self-explanatory.  If you want to connect to a QCC-server on a different port, then you can append the port at the end of the host in a common format like **host:port**, e.g.

    192.168.1.1:12345
    domain.com:12345

If you don't supply any port, the default port **54321** is used automatically.


## License

Copyright © 2011 Alexander Vos, Kai Wellmann.

Distributed under the GNU General Public License.  See
the COPYING file for more information.
