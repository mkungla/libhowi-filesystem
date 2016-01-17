<?php
/*******************************************************************
 * Created by:  Marko Kungla @ OkramLabs on Aug 6, 2012 - 9:07:40
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
 * File:      TmpInterface.inc
 * @link      https://
 ********************************************************************
 * Contributors:
 * @author Marko Kungla <okramlabs@gmail.com>
 *           Github: https://github.com/mkungla
 ********************************************************************
 * Comments:
 */
namespace HOWI3\libhowi\Filesystem\Commons\ObjectInterfaces;

interface TmpInterface
{

    /**
     * Set tmp path
     *
     * @param string $path            
     */
    public function setTmp($path = false);

    /**
     * Get tmp path
     *
     * @return string tmp path
     */
    public function getTmp();

    /**
     * * tempnam — Create file with unique file name
     *
     * @param string $dir            
     * @param string $prefix            
     *
     * @return string tmpname
     */
    public function tempnam($dir = false, $prefix = 'howi-fs-tmp-');

    /**
     * tmpfile — Creates a temporary file
     * 
     * @return filehandler
     */
    public function tmpfile();
}
