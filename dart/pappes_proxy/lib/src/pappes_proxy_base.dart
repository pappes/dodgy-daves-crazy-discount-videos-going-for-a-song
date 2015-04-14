// Copyright (c) 2015, pappes. All rights reserved. Use of this source code
// is governed by a BSD-style license that can be found in the LICENSE file.

/// The pappes_proxy library.
library pappes_proxy;


import 'package:http/http.dart' as http;
import 'package:shelf/shelf_io.dart' as shelf_io;
import 'package:shelf_proxy/shelf_proxy.dart';


/// Impliments a man-in-the-middle web server which emulates a proxy server
/// 
/// Uses [serverName] to determine the pulically addressable 
/// name of the server hosting the proxy. (defaults to localhost)
/// The port that the proxy server is addressible on 
/// needs to be passed into [serverPort]. (defaults to port 8080)
/// The content that is being proxied needs to be located at [targetUrl]. 
/// A custom httpClient handler can be passed in via [client].
/// You can name your proxy using [proxyName].
///
/// ## For example:
///    startProxy();
///    startProxy(serverName:'pappes.github.io',
///               serverPort:9000,
///               targetUrl:'http://www.microsoft.com',
///               client: new http.Client(),
///               proxyName: 'pappes_mitm_proxy');
void startProxy({String serverName, int serverPort, targetUrl, http.Client client, String proxyName}) {
  if (targetUrl == null) targetUrl = Uri.parse("https://www.google.com");
  if (serverPort == null) serverPort = 8080;
  if (serverName == null) serverName = 'localhost';
  
  shelf_io.serve(
      proxyHandler(targetUrl, client: client, proxyName: proxyName),
      serverName,
      serverPort).then((server) {
    print('Proxying at http://${server.address.host}:${server.port}');
  });
}

//pass in hostname and port
//impliment custom client to dereference query string
//change google.com to public server address
//do i need to change url.resolve?

//proxyhandler takes a string or it can take any object that extends Uri(specifically: resolve, authority, toString)
//serve  robots.txt to prevent web crawling
//impliment authentication client to limit connections and reduce overhead
