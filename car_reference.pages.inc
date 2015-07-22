<?php

/**
 * @file
 * Page callbacks for the car_reference module.
 */

/**
 * Page callback: Outputs JSON for taxonomy autocomplete suggestions.
 *
 * Path: taxonomy/autocomplete
 *
 * This callback outputs term name suggestions in response to Ajax requests
 * made by the taxonomy autocomplete widget for taxonomy term reference
 * fields. The output is a JSON object of plain-text term suggestions, keyed by
 * the user-entered value with the completed term name appended.  Term names
 * containing commas are wrapped in quotes.
 *
 * For example, suppose the user has entered the string 'red fish, blue' in the
 * field, and there are two taxonomy terms, 'blue fish' and 'blue moon'. The
 * JSON output would have the following structure:
 * @code
 *   {
 *     "red fish, blue fish": "blue fish",
 *     "red fish, blue moon": "blue moon",
 *   };
 * @endcode
 * 
 * @param string $entity_type
 *   The entity this field is attached to.
 *
 * @param string $bundle
 *   The bundle name for the instance..
 *
 * @param string $field_name
 *   The name of the term reference field.
 *
 * @param array $tags_typed
 *   (optional) A comma-separated list of term names entered in the
 *   autocomplete form element. Only the last term is used for autocompletion.
 *   Defaults to '' (an empty string).
 *
 * @see car_reference_menu()
 * @see car_reference_field_widget_info()
 */
function car_reference_autocomplete($entity_type, $bundle, $field_name, $tags_typed = '') {

  // If the request has a '/' in the search text, then the menu system will have
  // split it into multiple arguments, recover the intended $tags_typed.
  $args = func_get_args();
  // Slice off arguments other than $tags_typed.
  $args = array_slice($args, 3);
  $tags_typed = implode('/', $args);

  // Make sure the field exists and is a taxonomy field.
  if (!($field = field_info_field($field_name)) || $field['type'] !== 'car_reference') {
    // Error string. The JavaScript handler will realize this is not JSON and
    // will display it as debugging information.
    print t('Car reference field @field_name not found.', array('@field_name' => $field_name));
    exit;
  }

  // The user enters a comma-separated list of tags.
  // We only autocomplete the last tag.
  $tags_typed = drupal_explode_tags($tags_typed);
  $tag_last = drupal_strtolower(array_pop($tags_typed));

  $matches = array();
  if ($tag_last != '') {

    // Part of the criteria for the query come from the field's own settings.
    $vids = array();
    $vocabularies = taxonomy_vocabulary_get_names();

    // Get instance settings or display error.
    if (!$field_instance = field_info_instance($entity_type, $field_name, $bundle)) {
      print t('Unable to retrieve field instance settings for entity type: 
        "@entity_type", bundle: "@bundle", field name: "@field_name".', array(
          '@entity_type' => $entity_type,
          '@bundle' => $bundle,
          '@field_name' => $field_name,
        )
      );
      exit;
    }

    foreach ($field['settings']['allowed_values'] as $vocab_type => $tree) {
      // Only allow vocabs specified in field instance.
      $vocab_type = explode('_', $vocab_type);
      if (!empty($field_instance['settings']['choose_from'][$vocab_type[0]])) {
        $vids[] = $vocabularies[$tree['vocabulary']]->vid;
      }
    }
    // We must have at least one vocab to select terms from.
    if (empty($vids)) {
      print t('No valid vocabualaries to get terms from for this field instance. Allow either make, model or edition.');
      exit;
    }

    $query = db_select('taxonomy_term_data', 't');
    $query->addTag('translatable');
    $query->addTag('term_access');

    // Do not select already entered terms.
    if (!empty($tags_typed)) {
      $query->condition('t.name', $tags_typed, 'NOT IN');
    }
    // Select rows that match by term name.
    $tags_return = $query
      ->fields('t', array('tid', 'name'))
      ->condition('t.vid', $vids)
      ->condition('t.name', '%' . db_like($tag_last) . '%', 'LIKE')
      ->orderBy('name')
      ->range(0, 10)
      ->execute()
      ->fetchAllKeyed();

    $prefix = count($tags_typed) ? drupal_implode_tags($tags_typed) . ', ' : '';

    $term_matches = array();
    foreach ($tags_return as $tid => $name) {
      $n = $name;
      // Term names containing commas or quotes must be wrapped in quotes.
      if (strpos($name, ',') !== FALSE || strpos($name, '"') !== FALSE) {
        $n = '"' . str_replace('"', '""', $name) . '"';
      }
      $term_matches[$prefix . $n] = check_plain($name);
    }
  }

  drupal_json_output($term_matches);
}

/**
 * Get vocabulary fields.
 *
 * @param string $voc_name
 *   Vocabulary machine name.
 *
 * @return json
 */
function car_reference_vocabulary_fields($voc_name) {
  $vocabulary = taxonomy_vocabulary_machine_name_load($voc_name);

  if (!$vocabulary) {
    return;
  }

  $options = array();
  $vocabulary_fields = field_info_instances('taxonomy_term', $voc_name);

  foreach ($vocabulary_fields as $name => $field) {
    if ($field['entity_type'] == 'taxonomy_term') {
      $options[$name] = $name;
    }
  }

  if (empty($options)) {
    $options['_none'] = t('There are no available fields for this vocabulary.');
  }

  drupal_json_output($options);
}
