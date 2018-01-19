<?php

/**
 * @file
 * Documents hooks provided by this module.
 */


/**
 * Allows the autocomplete options to be modified.
 *
 * @param int $tid
 *   The term id.
 * @return array
 *   The list of modifiers.
 */
function hook_car_reference_autocomplete_modifier($tid) {
  // Add tid suffix to the term name.
  $term = $term = taxonomy_term_load($tid);

  return array($term->name . ' [' . $tid . ']');
}
