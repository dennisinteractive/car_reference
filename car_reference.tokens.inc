<?php

/**
 * @file
 * Tokens file for Car reference field module.
 */

/**
 * Implements hook_token_info().
 */
function car_reference_token_info() {
  $fields = car_reference_fields();
  foreach ($fields as $field_name => $field_config) {
    foreach ($field_config['settings']['allowed_values'] as $key => $settings) {
      $vocabulary = $settings['vocabulary'];
      // Expose the car reference term tokens.
      $info['tokens']['node'][$field_name . '_' . $vocabulary] = array(
        'name' => $field_name . ' ' . ucwords($vocabulary),
        'description' => 'Field "' . $field_name . '" reference to ' . ucwords($vocabulary),
        'type' => 'term',
      );
    }
  }
  return $info;
}

/**
 * Implements hook_tokens().
 */
function car_reference_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();

  if ($type == 'node' && !empty($data['node'])) {
    $node = $data['node'];
    // Provide replacement values for every possible car_reference field type.
    $fields = car_reference_fields();
    foreach ($fields as $field_name => $field_config) {
      foreach ($tokens as $name => $original) {
        // Expose tokens for each of the car terms referenced.
        foreach ($field_config['settings']['allowed_values'] as $key => $settings) {
          // Get vocabulary for this field value.
          $vocabulary = $settings['vocabulary'];
          // Check if this is a car reference token.
          $token_parts = explode(':', $name);
          $token_prefix = array_shift($token_parts);
          if ($token_prefix == $field_name . '_' . $vocabulary) {
            // Don't process token if term data isn't available.
            $car_reference_data = car_reference_get_node_car_reference_data($node, $field_name);
            if (isset($car_reference_data[$field_name][0][$vocabulary])) {
              // Convert to term token and replace.
              $term_token = '[term:' . implode(':', $token_parts) . ']';
              $replacements[$original] = token_replace($term_token, array('taxonomy_term' => $car_reference_data[$field_name][0][$vocabulary]));
            }
          }
        }
      }
    }
  }

  return $replacements;
}

/**
 * Fields that are of type 'car_reference'.
 * Cached result of field_read_fields(array('type' => 'car_reference'));
 */
function car_reference_fields() {
  static $fields;

  // Use the static cache.
  if (!is_null($fields)) {
    return $fields;
  }

  // Use the memcache.
  $cid = __FUNCTION__;
  if ($cache = cache_get($cid)) {
    $fields = $cache->data;
    return $fields;
  }

  // Get it from the db.
  $fields = field_read_fields(array('type' => 'car_reference'));

  // Cache it.
  cache_set($cid, $fields, CACHE_TEMPORARY);

  return $fields;
}
