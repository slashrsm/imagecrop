<?php
// $Id$

/**
 * @file
 * Form alter functions for Imagecrop
 *
 * @date
 * Nov 2, 2010
 */

/**
 * Implements hook_form_field_ui_field_edit_form_alter().
 * Add the imagecrop setting to an imagefield.
 */
function imagecrop_form_field_ui_field_edit_form_alter(&$form, $form_state) {

  if ($form['#field']['type'] == 'image') {
    $styles = get_imagecrop_styles();
    if (count($styles) > 0) {
      $form['instance']['settings']['imagecrop'] = array(
        '#type' => 'checkboxes',
        '#options' => $styles,
        '#title' => t('Available imagecrop styles'),
        '#description' => t('Select which imagecrop styles should be available for this field'),
        '#weight' => 12,
        '#default_value' => $form_state['build_info']['args'][0]['settings']['imagecrop'],
      );
    }

  }

}