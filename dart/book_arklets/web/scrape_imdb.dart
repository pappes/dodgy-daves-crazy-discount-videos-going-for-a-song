import 'dart:html';
import 'package:pappes_utility/pappes_utility.dart';
import 'package:pappes_web_utility/pappes_web_utility.dart';
import 'logging_functions.dart';

void main() {
  //use common function to set up logging
  turnOnLogging();
  //change all href Elements in the body to target the current tab
  final map = MyWebScraper.scrapeIMDB();  

    window.navigator.clipboard?.writeText(mapToCSV(map));
}