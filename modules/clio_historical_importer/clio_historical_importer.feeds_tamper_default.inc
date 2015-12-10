<?php
/**
 * @file
 * clio_historical_importer.feeds_tamper_default.inc
 */

/**
 * Implements hook_feeds_tamper_default().
 */
function clio_historical_importer_feeds_tamper_default() {
  $export = array();

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = 'countries_historical-country-html_entity_decode';
  $feeds_tamper->importer = 'countries_historical';
  $feeds_tamper->source = 'country';
  $feeds_tamper->plugin_id = 'html_entity_decode';
  $feeds_tamper->settings = array();
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'HTML entity decode';
  $export['countries_historical-country-html_entity_decode'] = $feeds_tamper;

  return $export;
}
