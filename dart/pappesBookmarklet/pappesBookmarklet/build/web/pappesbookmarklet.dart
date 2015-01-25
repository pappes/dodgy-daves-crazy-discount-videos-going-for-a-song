//import 'dart:js' as js;
import 'dart:html';
import 'package:pappes_web_utility/pappes_web_utility.dart';
//import 'package:pappes_utility/pappes_utility.dart';

removeRedirects(Event e) {
  MyHtml.iterateHTMLDOM(document.body, (element) => MyHtml.alterAttribute(element, 'href', (url) => MyHtml.removeUrlRedirect(url)));  
}


void main() {
  /*querySelector("#sample_text_id")
      ..text = "Click me!"
      ..onClick.listen(reverseText);*/
  

  querySelector('#generateButton').onClick.listen(removeRedirects);
}


/*void reverseText(MouseEvent event) {
  var text = querySelector("#sample_text_id").text;
  var buffer = new StringBuffer();
  for (int i = text.length - 1; i >= 0; i--) {
    buffer.write(text[i]);
  }
  querySelector("#sample_text_id").text = buffer.toString();
}*/
/*
/// Changes [originalURL] to remove any attempt at redirection.
///
/// ## Known redirect attempts:
/// * u=http://www.mysite.com
/// * u=www.mysite.com
/// * u=base64Encode(www.mysite.com)
/// * u=URLEncode(www.mysite.com)
String removeUrlRedirect(String originalUrl) {
  String finalUrl;
  String base64Uri;
  final Map params = Uri.parse(originalUrl).queryParameters;
  if (params is Map) for (String param in params.values) {
    // we have a web address so use it now
    if (param.toLowerCase().startsWith('www') || param.toLowerCase().startsWith('http')) {
      finalUrl = param;
      break;
    }
    //check that it is not null before using it.  Store it for later if it is a valid URL.
    if (base64Decode(param) != null && (base64Decode(param).toLowerCase().startsWith('www') || base64Decode(param).toLowerCase().startsWith('http'))) {
      base64Uri = base64Decode(param);
    }
  }  
  finalUrl = setUriSchemeToHttp(ifNull(finalUrl, base64Uri));
  finalUrl = ifNull(finalUrl, originalUrl);
  print ('''
removeUrlRediriect 
      originalUrl: $originalUrl 
  became finalUrl: $finalUrl''');
  return finalUrl;
}

/// Changes [originalURL] to use the http protocol if none specified.
///
/// This is used to unsure theat URLS that do not include the scheme 
/// are treated as absolute paths (not relative to the document URL).
String setUriSchemeToHttp(String originalURL) {
  String finalURL = originalURL;
  Uri parsedUri;
  try {
    parsedUri = Uri.parse(originalURL);
    if (parsedUri.scheme == null) finalURL = parsedUri.replace(scheme: 'http').toString();
  } catch (e) { //invalid URLs to be passed back unmodified
  }
  return finalURL;
}

/// Uses a javascript callout to convert [val] from base64 to text.
///
/// if [val] is not valid base64 then returns null
/// if library dart:js is not working throws StateError
String base64Decode(String val) {
  bool jsHasAtob;
  try {
    jsHasAtob = js.context.hasProperty('atob');
  } catch (e) {
    throw (new StateError('Dart -> JavaScript interop not initialised.  Try changing your html to include <script src="packages/browser/inteerop.js"></script>);  Original error: $e'));
  }
  if (jsHasAtob) try {
    return js.context.callMethod('atob', ["$val"]);
  } catch (e) {
    //ignore attepts to decode invalid strings
    //"InvalidCharacterError: Failed to execute 'atob' on 'Window': The string to be decoded is not correctly encoded."
    return null;
  }
  throw (new StateError('Dart -> JavaScript interop not accepting calls to atob().  Try changing your html to include <script src="packages/browser/inteerop.js"></script>);'));
}

/// Substitutes the value [alternate] if the value [test] is null.
String ifNull(test, alternate) {
  return test != null ? test : alternate;
}

/// Alters a specific [attribute] of a HTML element [node]
/// by applying function [alter] to the [attribute].
///
/// * [node] - any HTML element.
/// * [attribute] - the name of the attribute to be modifiied.
/// * [alter] - a function that takes in the old value and returns the new value.
alterAttribute(Element node, String attribute, Function alter) {
  if (node.attributes.containsKey(attribute)) node.attributes[attribute] = alter(node.attributes[attribute]);
}

/// Applies a [process] to every element of a HTML [DOM] element
/// (or any other HTML element).
void iterateHTMLDOM(Element DOM, Function process) {
  process(DOM);
  if (DOM.hasChildNodes()) {
    DOM.children.forEach((child) => iterateHTMLDOM(child, process));
  }
}*/
