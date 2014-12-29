import 'dart:js' as js;
import 'dart:html';

void main() {
  querySelector("#sample_text_id")
      ..text = "Click me!"
      ..onClick.listen(reverseText);
  iterateHTMLDOM(
      document.body,
      (element) => alterAttribute (
          element,
          'href',
          (url) => removeURLRedirect(url)));
}

void reverseText(MouseEvent event) {
  var text = querySelector("#sample_text_id").text;
  var buffer = new StringBuffer();
  for (int i = text.length - 1; i >= 0; i--) {
    buffer.write(text[i]);
  }
  querySelector("#sample_text_id").text = buffer.toString();
}

/// Changes [originalURL] to remove any attempta at redirection. 
/// 
/// ## Known redirect attempts:
/// * u=http://www.mysite.com
/// * u=www.mysite.com
/// * u=base64Encode(www.mysite.com)
/// * u=URLEncode(www.mysite.com)
String removeURLRedirect(String originalURL) {
  String wwwURI;
  String httpURI;
  String base64URI;
  Uri.parse(originalURL).queryParameters.forEach( (arg, val) => findURLRedirect(val));
  return originalURL;
}
String findURLRedirect(String val) {
  String wwwURI;
  String httpURI;
  String base64URI;
  String btoa64URI;
  //print(val);
  if (wwwURI == null && val.toLowerCase().startsWith('www')) return val;// wwwURI=val;
  if (httpURI == null && val.toLowerCase().startsWith('http')) return val;// httpURI=val;
  //btoa64URI=js.context.callMethod('atob', ["$val"]);
  //js.context.callMethod("alert", ["hello"]);
  if (base64URI == null && js.context.callMethod('btoa', ["$val"]).startsWith('www')) return val;// base64URI=val;
//  if (base64URI == null && js.context.callMethod('window.btoa(""$val"")').startsWith('www')) return val;// base64URI=val;
  //javascript:void(location.href=alert(window.atob("aHR0cDovL3d3dy5yZWFsdXJsLmNvbS90YXJnZXQvP2M9ZCZlPTEyMw==")))
  return originalURL;
}

/// Alters a specific [attribute] of a HTML element [node] 
/// by applying function [alter] to the [attribute].
///
/// * [node] - any HTML element.
/// * [attribute] - the name of the attribute to be modifiied.
/// * [alter] - a function that takes in the old value and returns the new value.
alterAttribute(Element node, String attribute, Function alter) { 
  if (node.attributes.containsKey(attribute)) 
    node.attributes[attribute] = alter(node.attributes[attribute]);
}

/// Applies a [process] to every element of a HTML [DOM] element 
/// (or any other HTML element).
void iterateHTMLDOM(Element DOM, Function process) {
  process(DOM);
  if (DOM.hasChildNodes()) {
    DOM.children.forEach((child) => iterateHTMLDOM(child, process));
  }
}
