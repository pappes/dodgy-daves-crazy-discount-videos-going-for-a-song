import 'dart:html';
import 'package:pappes_web_utility/pappes_web_utility.dart';

void main() {  
  MyHtml.iterateHTMLDOM(
      document.body, 
      (element) => MyHtml.alterAttribute(
          element, 
          'href', 
          (url) => MyHtml.setUriParameters(url)));  
}