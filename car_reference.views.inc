<?php

/**
 * @file Views functions for module.
 */

/**
 * Implements hook_field_views_data().
 *
 * Views integration for car_reference fields. Adds a term relationship to the
 * default field data.
 *
 * @see field_views_field_default_views_data()
 */
function car_reference_field_views_data($field) {
  $data = field_views_field_default_views_data($field);
  foreach ($data as $table_name => $table_data) {

    // Add the relationship only on the make tid field.
    $data[$table_name][$field['field_name'] . '_make_tid']['relationship'] = array(
      'handler' => 'views_handler_relationship',
      'base' => 'taxonomy_term_data',
      'base field' => 'tid',
      'label' => t('make term from !field_name', array('!field_name' => $field['field_name'])),
    );

    $data[$table_name][$field['field_name'] . '_make_tid']['filter']['handler'] = 'views_handler_filter_term_node_tid';
    $data[$table_name][$field['field_name'] . '_make_tid']['filter']['vocabulary'] = $field['settings']['allowed_values']['make_vocabulary']['vocabulary'];

    // Add the relationship only on the model tid field.
    $data[$table_name][$field['field_name'] . '_model_tid']['relationship'] = array(
      'handler' => 'views_handler_relationship',
      'base' => 'taxonomy_term_data',
      'base field' => 'tid',
      'label' => t('model term from !field_name', array('!field_name' => $field['field_name'])),
    );
    $data[$table_name][$field['field_name'] . '_model_tid']['filter']['handler'] = 'views_handler_filter_term_node_tid';
    $data[$table_name][$field['field_name'] . '_model_tid']['filter']['vocabulary'] = $field['settings']['allowed_values']['model_vocabulary']['vocabulary'];

    // Add the relationship only on the edition tid field.
    $data[$table_name][$field['field_name'] . '_edition_tid']['relationship'] = array(
      'handler' => 'views_handler_relationship',
      'base' => 'taxonomy_term_data',
      'base field' => 'tid',
      'label' => t('edition term from !field_name', array('!field_name' => $field['field_name'])),
    );
    $data[$table_name][$field['field_name'] . '_edition_tid']['filter']['handler'] = 'views_handler_filter_term_node_tid';
    $data[$table_name][$field['field_name'] . '_edition_tid']['filter']['vocabulary'] = $field['settings']['allowed_values']['edition_vocabulary']['vocabulary'];
  }

  return $data;
}
