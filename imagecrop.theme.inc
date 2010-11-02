<?php
// $Id$

/**
 * @file
 * Theming functions for showing the imagecrop pages.
 *
 * @date
 * Oct 7, 2010
 */

/**
 * Override the page theming for the imagecrop pages.
 */
function imagecrop_page($variables) {
  return drupal_render($variables['page']['content']);
}