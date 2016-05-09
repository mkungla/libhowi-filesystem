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
 * File:		TraitForLog.inc
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

trait TraitForResponse
{

    /**
     * Following is optional to have user id and username attached to log
     * Following can be anything you want for example your currently logged in
     * user info or system user running the script.
     *
     * @var unknown
     */
    private $UID = 0;

    private $username = 'anonymous';

    /**
     * Loglevel defaults to 4
     * so $EMERGENCY, $ALERT, $CRITICAL, $ERROR are loged
     *
     * @var int $loglevel 0 - 8 (none - all)
     */
    private $loglevel = 4;

    /**
     * Path to file if you want to log messages to file
     *
     * @var string $log_to_file
     */
    private $log_to_file = false;

    /**
     * 
     * @var responseObject as soon first msg is logged
     */
    protected $response = false;

    /**
     * System is unusable, something happend which should not happen
     *
     * @var array $EMERGENCY
     */
    private $EMERGENCY = array(
        1 => 'emergency',
        100 => 'You need minimum php version 5.4'
    );

    /**
     * Action must be taken immediately.
     *
     * Example: Entire website down, database unavailable, etc. This should
     * trigger the SMS alerts and wake you up.
     *
     * @var array $ALERT
     */
    private $ALERT = array(
        2 => 'alert',
        200 => 'No such file or directory'
    );

    /**
     * Critical conditions.
     *
     * Example: Application component unavailable, unexpected exception.
     *
     * @var array $CRITICAL
     */
    private $CRITICAL = array(
        3 => 'critical'
    );

    /**
     * Runtime errors that do not require immediate action but should typically
     * be logged and monitored.
     *
     * @var array $ERROR
     */
    private $ERROR = array(
        4 => 'error',
        400 => 'Logfile location is invalid'
    );

    /**
     * Exceptional occurrences that are not errors.
     *
     * Example: Use of deprecated APIs, poor use of an API, undesirable things
     * that are not necessarily wrong.
     *
     * @var array $WARNING
     */
    private $WARNING = array(
        5 => 'warning',
        500 => 'Changing the working directory failed!',
        501 => 'Changed the temporary directory failed! Destination not writable: ',
        502 => 'Permission denied or parent of directory does not exist if creating recursive directories (recursive = false) ',
        503 => 'Can not create the file, permission denied or parent directory does not exist : ',
        504 => 'Failed to create file : ',
        505 => 'Reading info - Permission denied. : ',
        506 => 'Failed to create structure from array!'
    );

    /**
     * Normal but significant events.
     *
     * @var array $NOTICE
     */
    private $NOTICE = array(
        6 => 'notice',
        601 => 'Called file object without filename!',
        602 => 'Called info object without basename!'
    );

    /**
     * Interesting events.
     *
     * Example: User logs in, SQL logs.
     *
     * @var array $INFO
     */
    private $INFO = array(
        7 => 'info',
        701 => 'Created new directory : ',
        702 => 'Removed directory : ',
        703 => 'Created new file : ',
        704 => 'Created new diectory structure : '
    );

    /**
     * Detailed debug information.
     *
     * @var array $DEBUG
     */
    private $DEBUG = array(
        8 => 'debug',
        800 => 'Creating directory with empty path should not be called',
        801 => 'libhowi-filesystem OK',
        802 => 'Tried to access not directly accessible function which should be private function',
        803 => 'Changed the temporary directory success',
        804 => 'Changed the temporaty directory by sys_get_temp_dir()',
        805 => 'Returned current temporaty directory : ',
        806 => 'Created new temporary name for file : ',
        807 => 'Loaded directory object',
        808 => 'Loaded file object',
        809 => 'Loaded info object',
        810 => 'Creating structure from array!'
    );

    /**
     * Add message to log
     *
     * @param array $LEVEL
     *            from private static $LEVEL;
     * @param number $code            
     * @param string $context            
     * @param string $append_msg            
     * @return void
     */
    private function addTrace($LEVEL, $code = 0, $context = "not specified", $append_msg = false, $to_file = false)
    {
        $this->response = new ResponseObject();
        
        $this->response->setCode($code);
        $this->response->setTime();
        $this->response->setMsg($this->{$LEVEL}[$code] . (! empty($append_msg) ? ' - ' . $append_msg : ''));
        $this->response->setUID($this->getUID());
        $this->response->setUsername($this->getUsername());
        $this->response->setContext($context);
        $this->response->setLevel($LEVEL);
        
        $logging = $this->getLogFile();
        if (! empty($logging) && ! empty($to_file))
            $this->logToFile($LEVEL);
    }

    /**
     * Write to the file
     */
    protected function logToFile($LEVEL)
    {
        $message = date("d-m-Y H:i:s");
        $message .= ' [lev] ' . str_pad($LEVEL, 9, " ", STR_PAD_RIGHT) . ' [/lev]';
        $message .= ' [UID] ' . str_pad($this->getUID(), 11, " ", STR_PAD_RIGHT) . ' [/UID]';
        $message .= ' [Uname] ' . str_pad($this->getUsername(), 15, " ", STR_PAD_RIGHT) . ' [/Uname]';
        $message .= '[mt] ' . str_pad($this->getTime(), 18, " ", STR_PAD_RIGHT) . ' [/mt]';
        $message .= '[code] ' . str_pad($this->getCode(), 5, " ", STR_PAD_RIGHT) . ' [/code]';
        $message .= '[msg] ' . $this->getMsg() . ' [/msg]' . "\n";
        
        $logging = $this->getLogFile();
        if (empty($logging) || ! is_dir(dirname($logging)))
            return false;
        return file_put_contents($logging, $message, FILE_APPEND);
    }

    /**
     * Set loglevel
     *
     * @param number $level            
     */
    public function setLogLevel($level = 0)
    {
        $this->loglevel = (in_array($level, range(0, 8))) ? $level : $this->loglevel;
    }

    /**
     * Set log file
     *
     * @param string $filepath            
     * @return boolean
     */
    public function setLogFile($filepath = false)
    {
        $logfilepath = $this->makeAbsolute($filepath);
        if (empty($filepath) || ! $this->isWritable(dirname($logfilepath))) {
            $this->error(400, 'setLogFile');
            return false;
        }
        $this->log_to_file = $logfilepath;
        return true;
    }

    /**
     * Get logfile
     *
     * @return string
     */
    public function getLogFile()
    {
        return $this->log_to_file;
    }

    /**
     * Get log level
     *
     * @return int
     */
    public function getLogLevel()
    {
        return $this->loglevel;
    }

    /**
     *
     * @return int userid
     */
    public function getUID()
    {
        return $this->UID;
    }

    /**
     * Return event microtime
     */
    public function getTime()
    {
        return is_object($this->response) ? $this->response->getTime() : microtime(true);
    }

    /**
     * Return username
     */
    public function getUsername()
    {
        return $this->username;
    }

    /**
     * Get response with or without log 
     *
     * @param string $key            
     * @return object
     */
    public function response()
    {
        return $this->response;
    }

    /**
     * Get last status
     *
     * @return int
     */
    public function getStatus()
    {
        return $this->response->getStatus();
    }

    /**
     * Get last logged message
     *
     * @return string
     */
    public function getMsg()
    {
        return $this->response->getMsg();
    }

    /**
     * Get last logged message code
     *
     * @return int
     */
    public function getCode()
    {
        return ! is_object($this->response) ? 0 : $this->response->getCode();
    }

    /**
     * Set last status
     *
     * @return int
     */
    
    /**
     * Set user id
     *
     * @param number $ID            
     * @return void
     */
    public function setUID($UID)
    {
        $this->UID = empty($UID) ? $this->UID : $UID;
    }

    /**
     * Set username
     *
     * @param string $username            
     * @return void
     */
    public function setUsername($username)
    {
        $this->username = empty($username) ? $this->username : $username;
    }

    /**
     * response setStatus
     * 
     * @param bool $status            
     */
    public function setStatus($status)
    {
        if (is_object($this->response))
            $this->response->setStatus($status);
    }

    /**
     * response setCode
     * 
     * @param bool $code            
     */
    public function setCode($code)
    {
        if (is_object($this->response))
            $this->response->setCode($code);
    }

    /**
     * System is unusable, something happend which should not happen
     *
     * @param int $code            
     * @param resource $context            
     * @param string $append_msg            
     */
    public function emergency($code = 1, $append_msg = false, $context = false)
    {
        $to_file = true;
        if ($this->getLogLevel() < 1)
            $to_file = false;
        
        $this->addTrace('EMERGENCY', $code, $context, $append_msg, $to_file);
    }

    /**
     * Action must be taken immediately.
     *
     * Example: Entire website down, database unavailable, etc. This should
     * trigger the SMS alerts and wake you up.
     *
     * @param int $code            
     * @param resource $context            
     * @param string $append_msg            
     * @return null
     */
    public function alert($code = 2, $append_msg = false, $context = false)
    {
        $to_file = true;
        if ($this->getLogLevel() < 2)
            $to_file = false;
        $this->addTrace('ALERT', $code, $context, $append_msg, $to_file);
    }

    /**
     * Critical conditions.
     *
     * Example: Application component unavailable, unexpected exception.
     *
     * @param int $code            
     * @param resource $context            
     * @param string $append_msg            
     * @return null
     */
    public function critical($code = 3, $append_msg = false, $context = false)
    {
        $to_file = true;
        if ($this->getLogLevel() < 3)
            $to_file = false;
        $this->addTrace('CRITICAL', $code, $context, $append_msg, $to_file);
    }

    /**
     * Runtime errors that do not require immediate action but should typically
     * be logged and monitored.
     *
     * @param int $code            
     * @param resource $context            
     * @param string $append_msg            
     *
     * @return null
     */
    public function error($code = 4, $append_msg = false, $context = false)
    {
        $to_file = true;
        if ($this->getLogLevel() < 4)
            $to_file = false;
        $this->addTrace('ERROR', $code, $context, $append_msg, $to_file);
    }

    /**
     * Exceptional occurrences that are not errors.
     *
     * Example: Use of deprecated APIs, poor use of an API, undesirable things
     * that are not necessarily wrong.
     *
     * @param int $code            
     * @param resource $context            
     * @param string $append_msg            
     * @return null
     */
    public function warning($code = 5, $append_msg = false, $context = false)
    {
        $to_file = true;
        if ($this->getLogLevel() < 5)
            $to_file = false;
        $this->addTrace('WARNING', $code, $context, $append_msg, $to_file);
    }

    /**
     * Normal but significant events.
     *
     * @param int $code            
     * @param resource $context            
     * @param string $append_msg            
     *
     * @return null
     */
    public function notice($code = 6, $append_msg = false, $context = false)
    {
        $to_file = true;
        if ($this->getLogLevel() < 6)
            $to_file = false;
        $this->addTrace('NOTICE', $code, $context, $append_msg, $to_file);
    }

    /**
     * Interesting events.
     *
     * Example: User logs in, SQL logs.
     *
     * @param int $code            
     * @param resource $context            
     * @param string $append_msg            
     *
     * @return null
     */
    public function info($code = 7, $append_msg = false, $context = false)
    {
        $to_file = true;
        if ($this->getLogLevel() < 7)
            $to_file = false;
        $this->addTrace('INFO', $code, $context, $append_msg, $to_file);
    }

    /**
     * Detailed debug information.
     *
     * @param int $code            
     * @param resource $context            
     * @param string $append_msg            
     *
     * @return null
     */
    public function debug($code = 8, $append_msg = false, $context = false)
    {
        $to_file = true;
        if ($this->getLogLevel() < 8)
            $to_file = false;
        $this->addTrace('DEBUG', $code, $context, $append_msg, $to_file);
    }
}
