import 'dart:html';
import 'package:pappes_utility/pappes_utility.dart';
import 'package:pappes_web_utility/pappes_web_utility.dart';
import 'logging_functions.dart';

void main() {
  //use common function to set up logging
  turnOnLogging();
  //change all href Elements in the body to target the current tab
  String result = 'events=\n';
  final events = MyWebScraper.scrapeAnywhereList();  
  for (final event in events){
    result += '${mapToCSV(event)}/n';
  }

    window.navigator.clipboard?.writeText(result);
}