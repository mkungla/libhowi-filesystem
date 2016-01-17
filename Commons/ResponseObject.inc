<?php
/********************************************************************
 * Created by:	Marko Kungla @ OkramLabs on Aug 6, 2012 - 9:12:38
 * Contact:		okramlabs@gmail.com - https://okramlabs.com
 * @copyright:	2015 OkramLabs - https://okramlabs.com
 * @license		MIT
 *
 * Package name: libhowi-filesystem
 * @category	 HOWI3
 * @package		 libhowi
 * @subpackage	 filesystem
 * 
 * Lang:		PHP
 * Encoding:	UTF-8
 * File:		ResponseObject.inc
 * @link		https://github.com/okramlabs/libhowi-filesystem
 ********************************************************************
 * Contributors:
 * @author		Marko Kungla <okramlabs@gmail.com>
 *  	Github:	https://github.com/mkungla
 ********************************************************************
 * Comments:
 *     1. This response trait is adjusted to follow structure of PSR-3. psr/log
 *     see - https://github.com/php-fig/log
 */
namespace HOWI3\libhowi\Filesystem\Commons;

final class ResponseObject
{

    /**
     * Filesystem status
     *
     * All filesystem methods have true/false states which are stored here
     * so it will only change when any messages are logged above minimum loglevel
     *
     * @var int $status
     */
    private $status = false;

    /**
     * Last log code
     *
     * Last logged message code
     *
     * @var int $code
     */
    private $code = 0;

    /**
     * Last log message
     *
     * Defaults to OK and further holds last logged message
     *
     * @var string $message
     */
    private $message = 'OK';

    /**
     * Following is optional to have user id and username attached to log
     * Following can be anything you want for example your currently logged in
     * user info or system user running the script.
     *
     * @var unknown
     */
    private $UID = 0;

    private $username = 'anonymous';

    private $context;

    private $level;


    public function setStatus($arg)
    {
        $this->status = is_bool($arg) ? $arg : false;
    }

    public function setCode($code)
    {
        $this->code = is_int($code) ? $code : 0;
    }

    public function setMsg($msg)
    {
        $this->message = is_string($msg) ? $msg : 'BUG';
    }

    public function setUID($UID)
    {
        $this->UID = is_int($UID) ? $UID : 'BUG';
    }

    public function setUsername($username)
    {
        $this->username = is_string($username) ? $username : 'BUG';
    }

    public function setTime()
    {
        $this->microtime = microtime(true);
    }

    public function setContext($context)
    {
        $this->context = $context;
    }

    public function setLevel($level)
    {
        $this->level = $level;
    }


    public function getStatus()
    {
        return $this->status;
    }

    public function getCode()
    {
        return $this->code;
    }

    public function getMsg()
    {
        return $this->message;
    }

    public function getUID()
    {
        return $this->UID;
    }

    public function getUsername()
    {
        return $this->username;
    }

    public function getTime()
    {
        return $this->microtime;
    }

    public function getContext()
    {
        return $this->context;
    }

    public function getLevel()
    {
        return $this->level;
    }
}
