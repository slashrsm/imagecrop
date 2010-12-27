<?php
// $Id$

/**
 * @file
 * Imagecrop crop UI template
 *
 * @date Dec 27, 2010
 *
 */
print drupal_render($style_selection);
print drupal_render($settings_form);
?>
<div id="imagecrop-crop-wrapper" style="width: <?php print $imagecrop->getImageWidth() ?>px; height: <?php print $imagecrop->getImageHeight() ?>px;">
  <div id="image-crop-container" style="background-image: url('<?php print $imagecrop->getFile()->filepath ?>'); width:<?php print $imagecrop->getImageWidth() ?>px; height:<?php print $imagecrop->getImageHeight() ?>px;"></div>
  <div id="resizeMe" style="background-image: url('<?php print $imagecrop->getFile()->filepath ?>'); width:<?php print $imagecrop->getWidth() ?>px; height:<?php print $imagecrop->getHeight() ?>px; top: 20px;"></div>
</div>

