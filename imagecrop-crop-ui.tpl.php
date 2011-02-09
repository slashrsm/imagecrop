<?php
// $Id$

/**
 * @file
 * Imagecrop crop UI template
 *
 * @date Dec 27, 2010
 *
 */
$style = $imagecrop->getImageStyle();
?>

<div id="imagecrop-selection" class="clearfix">
  <?php print drupal_render($style_selection); ?>
  <a href="#" onclick="javascript: Drupal.Imagecrop.changeViewedImage(<?php print $style['isid'] ?>); return false;" class="form-item imagecrop-form-link"><?php print t('Back to preview from this style') ?></a>
</div>

<div id="imagecrop-help">
  <?php print t("Resize image if needed, then select a crop area. Click 'Save selection' to save the changes."); ?>
</div>

<div id="imagecrop-forms" class="clearfix">
<?php
print drupal_render($settings_form);
print drupal_render($scale_form);
?>
</div>

<div id="imagecrop-crop-wrapper" style="width: <?php print $imagecrop->getImageWidth() ?>px; height: <?php print $imagecrop->getImageHeight() ?>px;">
  <div id="image-crop-container" style="background-image: url('<?php print $imagecrop->getCropDestination(); ?>'); width:<?php print $imagecrop->getImageWidth() ?>px; height:<?php print $imagecrop->getImageHeight() ?>px;"></div>
  <div id="resizeMe" style="background-image: url('<?php print $imagecrop->getCropDestination(); ?>'); width:<?php print $imagecrop->getWidth() ?>px; height:<?php print $imagecrop->getHeight() ?>px; top: 20px;"></div>
</div>

