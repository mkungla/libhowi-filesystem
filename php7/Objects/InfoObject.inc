<?php
/*******************************************************************
 * Created by:  Marko Kungla @ OkramLabs on Aug 6, 2012 - 9:52:48
 * Contact:     okramlabs@gmail.com - https://okramlabs.com
 * @copyright   2015 OkramLabs - https://okramlabs.com
 * @license     MIT
 *
 * Package name:libhowi-filesystem
 * @category	HOWI3
 * @package		libhowi
 * @subpackage	filesystem
 * 
 * Lang:      PHP
 * Encoding:  UTF-8
 * File:      InfoObject.inc
 * @link      https://
 ********************************************************************
 * Contributors:
 * @author Marko Kungla <okramlabs@gmail.com>
 *           Github: https://github.com/mkungla
 ********************************************************************
 * Comments:
 */
namespace HOWI3\libhowi\Filesystem\php7\Objects;

use \SplFileInfo;
use \HOWI3\libhowi\Filesystem\Commons\ObjectInterfaces\InfoInterface;
use \HOWI3\libhowi\Filesystem\php7\TraitForSharedMethods;


class InfoObject extends SplFileInfo implements InfoInterface
{
    use TraitForSharedMethods;

    public function getLinkTarget()
    {
        return $this->isLink() ? parent::getLinkTarget() : false;
    }
    
    public function getFileInfo($class_name = '\HOWI3\libhowi\Filesystem\php7\Objects\InfoObject')
    {
        return parent::getFileInfo($class_name);
    }
}