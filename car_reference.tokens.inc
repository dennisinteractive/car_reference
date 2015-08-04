<?php

/**
 * @file
 * Tokens file for Car reference field module.
 */

/**
 * Implements hook_token_info().
 */
function car_reference_token_info() {

  // Define a dedicated token type.
  $types['car_reference'] = array(
    'name' => t('Car Reference fields'),
    'description' => t('Tokens related to the Car Reference field type.'),
  );

  // Car reference specific tokens.
  $fields = field_read_fields(array('type' => 'car_reference'));
  foreach ($fields as $field_name => $field_config) {
    foreach ($field_config['settings']['allowed_values'] as $key => $settings) {
      $vocabulary = $settings['vocabulary'];
      // Expose the term name token.
      $car_references[$field_name . '_' . $vocabulary] = array(
        'name' => ucwords($vocabulary),
        'description' => ucwords($vocabulary),
        'type' => 'term',
      );
    }
  }

  return array(
    'types' => $types,
    'tokens' => array(
      'car_reference' => $car_references,
    ),
  );
}

/**
 * Implements hook_tokens().
 */
function car_reference_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();

  if ($type == 'car_reference' && !empty($data['node'])) {
    $node = $data['node'];
    // Provide replacement values for every possible car_reference field type.
    $fields = field_read_fields(array('type' => 'car_reference'));
    foreach ($fields as $field_name => $field_config) {
      $car_reference_data = car_reference_get_node_car_reference_data($node, $field_name);
      foreach ($tokens as $name => $original) {
        $fields = field_read_fields(array('type' => 'car_reference'));
        foreach ($fields as $field_name => $field_config) {
          // Expose tokens for each of the car terms referenced.
          foreach ($field_config['settings']['allowed_values'] as $key => $settings) {
            // Get vocabulary for this field value.
            $vocabulary = $settings['vocabulary'];
            // Don't process token if term data isn't available.
            if (!isset($car_reference_data[$vocabulary])) {
              continue;
            }
            // Convert to term token and replace.
            $token_parts = explode(':', $name);
            $token_prefix = array_shift($token_parts);
            if ($token_prefix == $field_name . '_' . $vocabulary) {
              $term_token = '[term:' . implode(':', $token_parts) . ']';
              $replacements[$original] = token_replace($term_token, array('taxonomy_term' => $car_reference_data[$vocabulary]));
            }
          }
        }
      }
    }
  }

  return $replacements;
}
