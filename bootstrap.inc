<?php
/********************************************************************
 * Created by:	Marko Kungla @ OkramLabs on Aug 6, 2012 - 8:46:17
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
 * File:		bootstrap.inc
 * @link		https://
 ********************************************************************
 * Contributors:
 * @author		Marko Kungla <okramlabs@gmail.com>
 *  	Github:	https://github.com/mkungla
 ********************************************************************
 * Comments:
 */

define("LIBHOWI_ROOT", dirname(__FILE__));

function libhowi_autoloder($class = false)
{
    $library = preg_replace(array(
        '/HOWI3\\\\libhowi\\\\Filesystem/',
        '/\\\\/'
    ), array(
        '',
        DIRECTORY_SEPARATOR
    ), $class);
    
    $class_file = LIBHOWI_ROOT . $library . '.inc';
    if (file_exists($class_file))
        require_once ($class_file);
}

spl_autoload_register('libhowi_autoloder');




