<?php
/********************************************************************
 * Created by:	Marko Kungla @ OkramLabs on Aug 6, 2012 - 9:42:04
 * Contact:		okramlabs@gmail.com - https://okramlabs.com
 * @copyright:	2015 OkramLabs - https://okramlabs.com
 * @license		MIT
 *
 * Package name:libhowi-filesystem
 * @category	HOWI3
 * @package		libhowi
 * @subpackage	filesystem
 *
 * Lang:		PHP
 * Encoding:	UTF-8
 * File:		InfoInterface.inc
 * @link		https://
 ********************************************************************
 * Contributors:
 * @author		Marko Kungla <okramlabs@gmail.com>
 *  	Github:	https://github.com/mkungla
 ********************************************************************
 * Comments:
 */
namespace HOWI3\libhowi\Filesystem\Commons\ObjectInterfaces;

interface InfoInterface
{

    /**
     * Gets the target of a link
     *
     * @return string target
     */
    public function getLinkTarget();

    /**
     * Gets an InfoObject object for the file
     *
     * @param string $class_name            
     */
    public function getFileInfo($class_name = '\HOWI3\libhowi\Filesystem\php5\Objects\InfoObject');
}
