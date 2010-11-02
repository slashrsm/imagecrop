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

/**
 * Implements hook_element_info_alter().
 */
function imagecrop_element_info_alter(&$type) {
  $type['managed_file']['#after_build'][] = 'process_imagecrop_form_element';
}

/**
 * Process function for imagecrop-enabled fields.
 */
function process_imagecrop_form_element($element) {

  // Leave alone if we are on the field ui settings.
  if (!isset($element['#field_name'])) {
    return $element;
  }

  // Leave element when user doesn't have access.
  if (!user_access('crop images with toolbox')) {
    return $element;
  }

  $enabled_styles = imagecrop_get_fields_enabled_styles($element['#entity_type'], $element['#field_name'], $element['#bundle']);

  // Leave element when no image styles are found for current field.
  if (count($enabled_styles) == 0) {
    return $element;
  }

  $element['imagecrop'] = array(
    '#type' => 'markup',
    '#weight' => 10,
    '#suffix' => '</div>',
  );

  if ($element['fid']['#value']) {
    $element['imagecrop']['#prefix'] = '<div class="imagecrop form-item container-inline">';
    $element['imagecrop']['#markup'] = imagecrop_linkitem($element['fid']['#value'], $element);
  }
  else {
    $element['imagecrop']['#prefix'] = '<div class="description">';
    $element['imagecrop']['#markup'] = t('After uploading an image you\'ll be able to crop it.');
  }

  return $element;

}