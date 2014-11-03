<?php

/**
 * @file
 * Contains \Drupal\imagecrop\ImageCropInterface.
 */

namespace Drupal\imagecrop;

use Drupal\Core\Entity\ContentEntityInterface;

/**
 * Provides an interface defining the image crop entity.
 */
interface ImageCropInterface extends ContentEntityInterface {

  /**
   * Gets file.
   *
   * @return \Drupal\file\FileInterface
   */
  public function file();

  /**
   * Gets image style.
   *
   * @return \Drupal\image\ImageStyleInterface
   */
  public function imageStyle();
}
