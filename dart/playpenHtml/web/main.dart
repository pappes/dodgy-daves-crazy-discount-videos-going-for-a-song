// Copyright (c) 2014, <your name>. All rights reserved. Use of this source code
// is governed by a BSD-style license that can be found in the LICENSE file.

import 'dart:html';
//import 'dart:async';

import 'package:pappes_web_utility/pappes_web_utility.dart';


//TODO if there is an iframe
  //load content into memory--getting difficult
  //set base to iframe source
  //render html into window
  


void whitelistElementAndParents(Element e, Set s ) {
  if (e.parent != null) whitelistElementAndParents(e.parent, s );
  s.remove(e);  
}

  String modifyHtml(String originalHtml) {
    return originalHtml..replaceAll('class', 'crass')
      ..replaceAll('setTimeout', 'dontSetTimeout')
      ..replaceAll('style', 'smile')
      ..replaceAll('position:', 'poison:')
      ..replaceAll('position%3A', 'poison%3A')
      ..replaceAll('onclick', 'oncrick')
      //..replaceAll('<style', '<!--')
      //..replaceAll('</style>', '-->')
      ;
  }

  
  void embedIFrameInBody (String contents){
    String fragment = '<iframe src=\'$contents\'>';
    fragment = modifyHtml(fragment);
    Element iframeElement = MyHtml.createElementFromHTML(fragment);
    document.body.append(iframeElement);    
  }
  
  buildFrameAsHtml(Element e) {
      String src;
      if (e.attributes['src'].contains('</html>'))
        embedIFrameInBody(e.attributes['src']);
      else{
        //unable to load external web site content
//        addIFrameToBody(' data:text/html,'+contents);
      }
      e.remove();
    }
  

  void stripDownCurrentPage() {
    Set<Element> elementsToBeDeleted = querySelectorAll('*').toSet();
  
    //whitelist all elements of type input so that the user can still search
    querySelectorAll('input').forEach((e) => whitelistElementAndParents(e, elementsToBeDeleted ));
    //whitelist all elements of type object so the user can wath videos
    querySelectorAll('object').forEach((e) => whitelistElementAndParents(e, elementsToBeDeleted ));
    //whitelist all elements of type iframe so that external content can remain
    querySelectorAll('iframe').forEach((e) => whitelistElementAndParents(e, elementsToBeDeleted ));
    //whitelist all elements of type anchor that have text so the user can click on links but not buttons
    querySelectorAll('a').forEach((Element e) {
      if (e.text != null) whitelistElementAndParents(e, elementsToBeDeleted );
    });  
    //destroy everything that remains
    elementsToBeDeleted.forEach((Element e) => e.remove());


  //pull down each iframe and add it to the body
    //querySelectorAll('iframe').toList().forEach(buildFrameAsHtml);

  //pull down each iframe and add it to the body
  for (Element e in querySelectorAll('iframe').where((e) => e.width > 400)) {
    if (Uri.parse(window.location.href).host != Uri.parse(e.attributes['src']).host) {
      window.location.assign(e.attributes['src']);
      break;
  }
  }
  }
//      var client = new http.BrowserClient();
//      client.read(e.attributes['src']).then((contents) { 
//        addIFrameToBody(' data:text/html,'+contents);
//      });
      //TODO load the html into a variable
    //http browserClient works only if the foreign site allows 'Access-Control-Allow-Origin'
//      var client = new http.BrowserClient();
//      client.read(e.attributes['src']).then((contents) { 
//        addIFrameToBody(' data:text/html,'+contents);
//      });
    //HttpRequest works only if the foreign site allows 'Access-Control-Allow-Origin'
    //HttpRequest.request(e.attributes['src']).then((contents) => addIFrameToBody(' data:text/html,'+contents));
  
  
  
 /* 
  Window extSite = new Window();
  String iFrameHtml =  '<iframe src=\' data:text/html,'+
'''<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<!--!DOCTYPE html-->

<!--
  Copyright (c) 2014, <your name>. All rights reserved. Use of this source code
  is governed by a BSD-style license that can be found in the LICENSE file.
-->

<html>
<head>
    <title>playpenHtml</title>

    <link rel="stylesheet" href="styles/main.css">
</head>

<body>


  <div id="leftBlockiframe" class="left bright overlay">
    <ul>
      <li>html left item 1</li>
      <li>html left item 2</li>
      <li>html left item 3</li>
    </ul>
  </div>

  <div id="rightBlockiframe" class="right bright underlay">
    <ul>
      <li>html right item 1</li>
      <li>html right item 2</li>
      <li>html right item 3</li>
    </ul>
    <div>
  <a>x </a>
  <object>myobject </object></div>
  </div>
</body>
</html>
'''
      +'\'>';

    document.body.append(MyHtml.createElementFromHTML(iFrameHtml));
*/
  

  //print('still running');



void main() {
  querySelector('#output').text = 'Dart is running.';

}