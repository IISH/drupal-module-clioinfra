<?php
/**
 * @file
 * clio_historical_importer.feeds_importer_default.inc
 */

/**
 * Implements hook_feeds_importer_default().
 */
function clio_historical_importer_feeds_importer_default() {
  $export = array();

  $feeds_importer = new stdClass();
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'countries_historical';
  $feeds_importer->config = array(
    'name' => 'Countries (historical)',
    'description' => 'Sync historical countries from an external source (flat)',
    'fetcher' => array(
      'plugin_key' => 'FeedsHTTPFetcher',
      'config' => array(
        'auto_detect_feeds' => FALSE,
        'use_pubsubhubbub' => FALSE,
        'designated_hub' => '',
        'request_timeout' => NULL,
        'auto_scheme' => 'http',
        'accept_invalid_cert' => FALSE,
      ),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsExJsonPath',
      'config' => array(
        'context' => array(
          'value' => '$.*',
        ),
        'sources' => array(
          'country' => array(
            'name' => 'year',
            'value' => 'year',
            'debug' => 0,
            'weight' => '1',
          ),
          'ccode' => array(
            'name' => 'id',
            'value' => 'id',
            'debug' => 0,
            'weight' => '2',
          ),
        ),
        'display_errors' => 1,
        'debug_mode' => 1,
        'source_encoding' => array(
          0 => 'UTF-8',
        ),
      ),
    ),
    'processor' => array(
      'plugin_key' => 'FeedsTermProcessor',
      'config' => array(
        'vocabulary' => 0,
        'mappings' => array(
          0 => array(
            'source' => 'country',
            'target' => 'name',
            'unique' => 1,
          ),
          1 => array(
            'source' => 'ccode',
            'target' => 'field_country_id',
            'unique' => FALSE,
          ),
          2 => array(
            'source' => 'ccode',
            'target' => 'guid',
            'unique' => 1,
          ),
        ),
        'update_existing' => '2',
        'input_format' => 'plain_text',
        'skip_hash_check' => 0,
        'bundle' => 'countries_historical',
        'update_non_existent' => 'skip',
        'insert_new' => '1',
      ),
    ),
    'content_type' => '',
    'update' => 0,
    'import_period' => '86400',
    'expire_period' => 3600,
    'import_on_create' => 1,
    'process_in_background' => 0,
  );
  $export['countries_historical'] = $feeds_importer;

  return $export;
}
