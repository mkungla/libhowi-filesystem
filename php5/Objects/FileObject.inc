<?php
/*******************************************************************
 * Created by:  Marko Kungla @ OkramLabs on Aug 6, 2012 - 10:02:48
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
 * File:      fileObject.inc
 * @link      https://
 ********************************************************************
 * Contributors:
 * @author Marko Kungla <okramlabs@gmail.com>
 *           Github: https://github.com/mkungla
 ********************************************************************
 * Comments:
 */
namespace HOWI3\libhowi\Filesystem\php5\Objects;

use \SplFileObject;
use \HOWI3\libhowi\Filesystem\Commons\ObjectInterfaces\FileInterface;
use \HOWI3\libhowi\Filesystem\php5\TraitForSharedMethods;

class FileObject extends SplFileObject implements FileInterface
{
    use TraitForSharedMethods;

    /**
     *
     * {@inheritDoc}
     *
     */
    public function getLinkTarget()
    {
        return false;
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function flock($operation, $wouldblock = false)
    {
        return ! $wouldblock ? parent::flock($operation) : parent::flock($operation, $wouldblock);
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function hasChildren()
    {
        return false;
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function getChildren()
    {
        return false;
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function isDot()
    {
        return false;
    }
    
    /**
     * Dummy file scale up to x bytes
     * @param int $size
     */
    public function dummyScale($size)
    {
        if(!is_int($size) || $this->getSize() > $size) return false;
        
        $this->fseek($size ,SEEK_SET);
        $this->fwrite(' ');
        return true;
    }
}
