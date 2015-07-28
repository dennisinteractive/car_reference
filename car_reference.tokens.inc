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
  foreach (_get_car_reference_fields() as $field_name) {
    $car_references[$field_name . '_make_name'] = array(
      'name' => t('Make name'),
      'description' => t('Make name.'),
    );
    $car_references[$field_name . '_make_name_url'] = array(
      'name' => t('Make name for URLs'),
      'description' => t('Make name value for use in URLs.'),
    );

    $car_references[$field_name . '_model_name'] = array(
      'name' => t('Model name'),
      'description' => t('Model name.'),
    );
    $car_references[$field_name . '_model_name_url'] = array(
      'name' => t('Model name for URLs'),
      'description' => t('Model name value for use in URLs.'),
    );

    $car_references[$field_name . '_edition_name'] = array(
      'name' => t('Edition name'),
      'description' => t('Edition name.'),
    );
    $car_references[$field_name . '_edition_name_url'] = array(
      'name' => t('Edition name for URLs'),
      'description' => t('Edition name value for use in URLs.'),
    );
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
    foreach (_get_car_reference_fields() as $field_name) {
      $car_reference_data = car_reference_get_node_car_reference_data($node, $field_name);
      foreach ($tokens as $name => $original) {
        $token_name = str_replace($field_name . '_', '', $name);
        switch ($token_name) {
          case 'make_name':
          case 'make_name_url':
            if (!empty($car_reference_data['make'])) {
              $term = reset($car_reference_data['make']);
              $replacements[$original] = $term['name'];
            }
            break;

          case 'model_name':
          case 'model_name_url':
            if (!empty($car_reference_data['model'])) {
              $term = reset($car_reference_data['model']);
              $replacements[$original] = $term['name'];
            }
            break;

          case 'edition_name':
          case 'edition_name_url':
            if (!empty($car_reference_data['edition'])) {
              $term = reset($car_reference_data['edition']);
              $replacements[$original] = $term['name'];
            }
            break;
        }

        if (!empty($replacements[$original]) && strpos($name, '_name_url') !== FALSE) {
          if (!function_exists('pathauto_cleanstring')) {
            module_load_include('inc', 'pathauto');
          }
          $replacements[$original] = pathauto_cleanstring($replacements[$original]);
        }

      }
    }
  }

  return $replacements;
}
