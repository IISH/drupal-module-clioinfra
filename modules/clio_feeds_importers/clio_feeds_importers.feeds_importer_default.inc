<?php
/**
 * @file
 * clio_feeds_importers.feeds_importer_default.inc
 */

/**
 * Implements hook_feeds_importer_default().
 */
function clio_feeds_importers_feeds_importer_default() {
  $export = array();

  $feeds_importer = new stdClass();
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'countries';
  $feeds_importer->config = array(
    'name' => 'Countries',
    'description' => 'Sync countries from an external source',
    'fetcher' => array(
      'plugin_key' => 'FeedsHTTPFetcher',
      'config' => array(
        'auto_detect_feeds' => FALSE,
        'use_pubsubhubbub' => FALSE,
        'designated_hub' => '',
        'request_timeout' => NULL,
      ),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsExJsonPath',
      'config' => array(
        'context' => array(
          'value' => '$.oecd.*',
        ),
        'sources' => array(
          'country' => array(
            'name' => 'country',
            'value' => 'country',
            'debug' => 0,
            'weight' => '1',
          ),
          'region' => array(
            'name' => 'region',
            'value' => 'region',
            'debug' => 0,
            'weight' => '2',
          ),
          'ccode' => array(
            'name' => 'ccode',
            'value' => 'ccode',
            'debug' => 0,
            'weight' => '3',
          ),
        ),
        'display_errors' => 1,
        'debug_mode' => 1,
        'source_encoding' => array(
          0 => 'auto',
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
          3 => array(
            'source' => 'region',
            'target' => 'parent',
            'unique' => 1,
          ),
        ),
        'update_existing' => '2',
        'input_format' => 'plain_text',
        'skip_hash_check' => 0,
        'bundle' => 'countries',
      ),
    ),
    'content_type' => '',
    'update' => 0,
    'import_period' => 1800,
    'expire_period' => 3600,
    'import_on_create' => TRUE,
    'process_in_background' => FALSE,
  );
  $export['countries'] = $feeds_importer;

  $feeds_importer = new stdClass();
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'indicators';
  $feeds_importer->config = array(
    'name' => 'Indicators',
    'description' => 'Sync indicators from an external source',
    'fetcher' => array(
      'plugin_key' => 'FeedsHTTPFetcher',
      'config' => array(
        'auto_detect_feeds' => FALSE,
        'use_pubsubhubbub' => FALSE,
        'designated_hub' => '',
        'request_timeout' => NULL,
      ),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsExJsonPath',
      'config' => array(
        'context' => array(
          'value' => '$.*',
        ),
        'sources' => array(
          'indicator' => array(
            'name' => 'indicator',
            'value' => 'indicator',
            'debug' => 0,
            'weight' => '1',
          ),
          'id' => array(
            'name' => 'id',
            'value' => 'id',
            'debug' => 0,
            'weight' => '2',
          ),
        ),
        'display_errors' => 1,
        'debug_mode' => 1,
        'source_encoding' => array(
          0 => 'auto',
        ),
      ),
    ),
    'processor' => array(
      'plugin_key' => 'FeedsTermProcessor',
      'config' => array(
        'vocabulary' => 0,
        'mappings' => array(
          0 => array(
            'source' => 'indicator',
            'target' => 'name',
            'unique' => 1,
          ),
          1 => array(
            'source' => 'id',
            'target' => 'field_indicator_id',
            'unique' => FALSE,
          ),
          2 => array(
            'source' => 'topic',
            'target' => 'parent',
            'unique' => 1,
          ),
          3 => array(
            'source' => 'id',
            'target' => 'guid',
            'unique' => FALSE,
          ),
        ),
        'update_existing' => '2',
        'input_format' => 'plain_text',
        'skip_hash_check' => 0,
        'bundle' => 'indicators',
      ),
    ),
    'content_type' => '',
    'update' => 0,
    'import_period' => 1800,
    'expire_period' => 3600,
    'import_on_create' => TRUE,
    'process_in_background' => FALSE,
  );
  $export['indicators'] = $feeds_importer;

  return $export;
}