<?php
use HOWI3\libhowi\Filesystem\FS;

class FilesystemTests extends PHPUnit_Framework_TestCase
{

    protected $FS;

    protected function setUp()
    {
        if ($this->FS instanceof HOWI3\libhowi\Filesystem\FS)
            return;
        
        $this->FS = FS::load();
        $logfile = $this->FS->makeAbsolute(FS_TEST_LOG_LOCATION);
        $this->FS->setLogLevel(FS_TEST_LOG_LEVEL);
        $this->FS->setUID(getmyuid());
        $this->FS->setUsername(get_current_user());
        $this->FS->setLogFile($logfile);
    }

    protected function addWarning($msg, Exception $previous = null)
    {
        $add_warning = $this->getTestResultObject();
        $msg = new PHPUnit_Framework_Warning($msg, 0, $previous);
        $add_warning->addWarning($this, $msg, time());
        $this->setTestResultObject($add_warning);
    }

    public function test_filesystem()
    {
        $this->assertInstanceOf('\HOWI3\libhowi\Filesystem\Commons\FilesystemInterface', $this->FS);
    }

    public function test_makeAbsolute()
    {
        $HOME = getenv("HOME");
        
        $this->FS->cd(FS_TEST_LOCATION);
        // makeAbsolute($path = false);
        $this->assertEquals($HOME . '/.HOWI/tmp', $this->FS->makeAbsolute('../'), 'Failed to assert ../');
        $this->assertEquals('/', 
            $this->FS->makeAbsolute('../../../../../../../../../../../../../../../../../../../../../../'), 
            'Failed to assert ../../../../../../../../../../../../../../../../../../../../../../');
        
        $this->FS->cd($HOME);
        
        $this->assertEquals('', $this->FS->makeAbsolute(''), 
            'Making absoulte path from empty string must return same string');
        $this->assertFalse($this->FS->makeAbsolute(), 'Making absoulte path with null must return false');
        
        $this->assertEquals($HOME, $this->FS->makeAbsolute('./'), './ must match CWD');
        $this->assertEquals($HOME . '/etc', $this->FS->makeAbsolute('~/etc'), 'Failed to assert ~/etc');
        $this->assertEquals($HOME . '/tmp', $this->FS->makeAbsolute('tmp'), 'Failed to assert tmp');
        $this->assertEquals($HOME . '/tmp/libhowi-filesystem', 
            $this->FS->makeAbsolute('tmp/libhowi-filesystem'), 'Failed to assert tmp/libhowi-filesystem');
        
        $this->assertEquals($HOME, $this->FS->makeAbsolute('.'), 'Failed to assert .');
        $this->assertEquals($HOME, $this->FS->makeAbsolute('./'), 'Failed to assert ./');
        $this->assertEquals($HOME . '/.HOWI/tmp/[{\-_)(+=~`!@#$%^&*()_-+={}[]|\?><,.', 
            $this->FS->makeAbsolute('~/.HOWI/tmp/[{\-_)(+=~`!@#$%^&*()_-+={}[]|\?><,.'), 'Failed to assert .');
    }

    public function test_FS_getCwd()
    {
        $this->assertInternalType('string', FS::load()->getCwd(), 
            "Current working directoroy must be always type of string!");
    }

    public function test_FS_cd_logical_nav_relative()
    {
        /* Logical Navigation */
        $FS = FS::load();
        $test = $FS->cd('../../../');
        $this->assertInternalType('boolean', $test, "CD navigation  must be always type of boolean!");
        $this->assertTrue($test, "Changing directory should not return false on logical navigation. ");
    }

    public function test_FS_cd_logical_nav_relative_name_validate()
    {
        /* Must be a dir named 'tests */
        $FS = FS::load(__DIR__);
        $test = $FS->cd('../', 'tests');
        $this->assertInternalType('boolean', $test, "CD navigation  must be always type of boolean!");
        $this->assertTrue($test, 
            "Changing directory should not return false on logical navigation and directory name validation. :");
    }

    public function test_FS_cd_logical_nav_relative_not_exist()
    {
        $FS = FS::load(__DIR__);
        $test = $FS->cd('../lets/try/to/go/some/randome/place/which/do/not/exist');
        $this->assertInternalType('boolean', $test, "CD navigation  must be always type of boolean!");
        $this->assertFalse($test, 
            "Changing directory must return false on logical navigation while destination does not exist. :");
    }

    public function test_getCwd()
    {
        $sysCD = getenv("HOME") . '/.HOWI/tmp/libhowi-filesystem';
        
        $this->assertTrue($this->FS->cd('~/.HOWI/tmp/libhowi-filesystem'), 'Could not CD to testing directory');
        $this->assertEquals($sysCD, $this->FS->getCwd(), 'Can not ensure correct navigation.');
        
        $this->assertFalse($this->FS->cd('/root'), 
            'setCwd must return false when trying to set non permited location as working dir');
    }

    public function test_isAbsolute()
    {
        $this->assertTrue($this->FS->isAbsolute('/'));
        $this->assertTrue($this->FS->isAbsolute('/home'));
        $this->assertTrue($this->FS->isAbsolute('/home/username'));
        
        $this->assertFalse($this->FS->isAbsolute('/home/../username'));
        $this->assertFalse($this->FS->isAbsolute('home'));
        $this->assertFalse($this->FS->isAbsolute('../'));
        $this->assertFalse($this->FS->isAbsolute('../home'));
        $this->assertFalse($this->FS->isAbsolute('c:home'));
        $this->assertFalse($this->FS->isAbsolute('.'));
        $this->assertFalse($this->FS->isAbsolute('https://www.example.com'));
        $this->assertFalse($this->FS->isAbsolute());
    }

    public function test_isRelative()
    {
        $this->assertTrue($this->FS->isRelative('tmp'), 'tmp is relative');
        $this->assertTrue($this->FS->isRelative('123tmp'), '123tmp is relative');
        $this->assertTrue($this->FS->isRelative('../tmp'), '../tmp is relative');
        $this->assertTrue($this->FS->isRelative('./tmp'), './tmp is relative');
        $this->assertTrue($this->FS->isRelative('~/.HOWI/tmp'), '~/.HOWI/tmp is relative');
        $this->assertTrue($this->FS->isRelative('.'), '. is relative');
        $this->assertTrue($this->FS->isRelative('..'), '.. is relative');
        $this->assertFalse($this->FS->isRelative('/home'), '/home is not relative');
        $this->assertFalse($this->FS->isRelative('/home/tmp'), '/home/tmp is not relative');
        $this->assertFalse($this->FS->isRelative(), 'Empty string is not relative');
        $this->assertFalse($this->FS->isRelative('/home/../tmp'), '/home/../tmp is relative');
    }

    public function test_isWritable()
    {
        $this->assertTrue($this->FS->isWritable('~/.HOWI/tmp'));
        $this->assertFalse($this->FS->isWritable('/'));
    }

    public function test_isReadable()
    {
        $this->assertTrue($this->FS->isReadable('~/.HOWI/tmp'), '~/.HOWI/tmpp should be readable');
        $this->assertTrue($this->FS->isReadable(FS_TEST_LOG_LOCATION), 'tmp log file should be readable');
        $this->assertFalse($this->FS->isReadable('/root'), 'root should not be readable');
    }

    public function test_set_get_LogLevel()
    {
        $this->FS->setLogLevel(8);
        $this->assertEquals(8, $this->FS->getLogLevel());
        
        $this->FS->setLogLevel(7);
        $this->assertEquals(7, $this->FS->getLogLevel());
    }

    public function test_set_get_LogFile()
    {
        $this->FS->setLogFile('/tmp/libhowi-filesystem.log');
        $this->assertEquals('/tmp/libhowi-filesystem.log', $this->FS->getLogFile());
        
        $this->FS->setLogFile(FS_TEST_LOG_LOCATION);
        $this->assertEquals($this->FS->makeAbsolute(FS_TEST_LOG_LOCATION), $this->FS->getLogFile());
    }

    public function test_get_set_UID()
    {
        $this->FS->setUID(10);
        $this->assertEquals(10, $this->FS->getUID());
        
        $this->FS->setUID(getmyuid());
        $this->assertEquals(getmyuid(), $this->FS->getUID());
    }

    public function test_getTime()
    {
        $this->assertInternalType('float', $this->FS->getTime());
    }

    public function test_set_get_Username()
    {
        $this->FS->setUsername(get_current_user());
        $this->FS->response();
        $this->assertEquals(get_current_user(), $this->FS->getUsername(), 
            'Validating response Username failed');
    }

    public function test_response()
    {
        $this->FS->debug(8, 'Testing response');
        $this->assertInstanceOf("HOWI3\libhowi\Filesystem\Commons\ResponseObject", $this->FS->response());
    }

    public function test_get_set_Status()
    {
        $this->assertInternalType('boolean', $this->FS->getStatus(), "Response Status must be always int!");
        $this->assertTrue($this->FS->getStatus());
        
        $this->FS->setStatus(false);
        $this->assertFalse($this->FS->getStatus());
    }

    public function test_getMsg()
    {
        $this->assertInternalType('string', $this->FS->getMsg(), 
            "Response Message must be always type of string!");
        $this->assertEquals('libhowi-filesystem OK', $this->FS->getMsg(), 
            "Default response message must be string 'OK' ");
    }

    public function test_get_set_Code()
    {
        $this->assertInternalType('int', $this->FS->getCode(), "Response Code must be always type of int!");
        $this->assertEquals(801, $this->FS->getCode());
        
        $this->FS->setCode(10000);
        $this->assertEquals(10000, $this->FS->getCode());
    }

    public function test_emergency()
    {
        $this->FS->emergency();
        $this->assertEquals(1, $this->FS->getCode());
        $this->assertEquals('emergency', $this->FS->getMsg());
    }

    public function test_alert()
    {
        $this->FS->alert();
        $this->assertEquals(2, $this->FS->getCode());
        $this->assertEquals('alert', $this->FS->getMsg());
    }

    public function test_critical()
    {
        $this->FS->critical();
        $this->assertEquals(3, $this->FS->getCode());
        $this->assertEquals('critical', $this->FS->getMsg());
    }

    public function test_error()
    {
        $this->FS->error();
        $this->assertEquals(4, $this->FS->getCode());
        $this->assertEquals('error', $this->FS->getMsg());
    }

    public function test_warning()
    {
        $this->FS->warning();
        $this->assertEquals(5, $this->FS->getCode());
        $this->assertEquals('warning', $this->FS->getMsg());
    }

    public function test_notice()
    {
        $this->FS->notice();
        $this->assertEquals(6, $this->FS->getCode());
        $this->assertEquals('notice', $this->FS->getMsg());
    }

    public function test_info()
    {
        $this->FS->info();
        $this->assertEquals(7, $this->FS->getCode());
        $this->assertEquals('info', $this->FS->getMsg());
    }

    public function test_debug()
    {
        $this->FS->debug();
        $this->assertEquals(8, $this->FS->getCode());
        $this->assertEquals('debug', $this->FS->getMsg());
    }

    public function test_cd()
    {
        $current_cwd = $this->FS->getCwd();
        
        $this->assertTrue($this->FS->cd(), 
            'Giving no arguments will return true since
            it will stay at same working dir or if working dir is not set then get env cwd');
        
        $this->assertFalse($this->FS->cd('/root'), 
            'Navigating to the non existing or non permitted location must return false');
        
        $this->assertEquals($current_cwd, $this->FS->getCwd());
        
        $this->FS->cd('../');
        $this->assertEquals(dirname($current_cwd), $this->FS->getCwd());
    }

    public function test_rm()
    {
        $this->assertFalse($this->FS->exists(FS_TEST_LOCATION . DIRECTORY_SEPARATOR . 'testrmdir'));
        $this->FS->dir('testrmdir', FS_TEST_LOCATION);
        $this->assertTrue($this->FS->exists(FS_TEST_LOCATION . DIRECTORY_SEPARATOR . 'testrmdir'));
        
        $this->FS->rm(FS_TEST_LOCATION . DIRECTORY_SEPARATOR . 'testrmdir');
        $this->assertFalse($this->FS->exists(FS_TEST_LOCATION . DIRECTORY_SEPARATOR . 'testrmdir'));
        
        $this->assertFalse($this->FS->exists(FS_TEST_LOCATION . DIRECTORY_SEPARATOR . 'testrmfile'));
        $this->FS->file('testrmfile', FS_TEST_LOCATION);
        $this->assertTrue($this->FS->exists(FS_TEST_LOCATION . DIRECTORY_SEPARATOR . 'testrmfile'));
        
        $this->FS->rm(FS_TEST_LOCATION . DIRECTORY_SEPARATOR . 'testrmfile');
        $this->assertFalse($this->FS->exists(FS_TEST_LOCATION . DIRECTORY_SEPARATOR . 'testrmfile'));
    }

    public function test_scandir()
    {
        $this->assertFalse($this->FS->scandir());
        $this->assertFalse($this->FS->scandir(FS_TEST_LOG_LOCATION));
        
        $this->assertInternalType('array', $this->FS->scandir($this->FS->getCwd()));
        $this->assertInternalType('array', $this->FS->scandir(__DIR__));
    }

    public function test_stat()
    {
        $this->assertFalse($this->FS->stat(), 'Calling stat with null or empty string should return false');
        $this->assertInternalType('array', $this->FS->stat($this->FS->getCwd()));
        $this->assertInternalType('array', $this->FS->stat(FS_TEST_LOG_LOCATION));
        $this->assertInternalType('array', $this->FS->stat(__DIR__));
        $this->assertInternalType('array', $this->FS->stat(__FILE__));
    }

    public function test_getDiskTotalSpace()
    {
        // getDiskTotalSpace($partition_location = false, $convert = false);
        $this->assertInternalType('float', $this->FS->getDiskTotalSpace(), 
            'Default getDiskTotalSpace should return float');
        
        $this->assertInternalType('float', $this->FS->getDiskTotalSpace("/"), 
            'Default getDiskTotalSpace should return float');
        
        $this->assertInternalType('array', $this->FS->getDiskTotalSpace("/", true), 
            'getDiskTotalSpace with second parameter true should return array');
    }

    public function test_getDiskFreeSpace()
    {
        // getDiskFreeSpace($partition_location = false, $convert = false);
        $this->assertInternalType('float', $this->FS->getDiskFreeSpace(), 
            'Default getDiskTotalSpace should return float');
        $this->assertInternalType('float', $this->FS->getDiskFreeSpace("/"), 
            'Default getDiskTotalSpace should return float');
        $this->assertInternalType('array', $this->FS->getDiskFreeSpace("/", true), 
            'getDiskTotalSpace with second parameter true should return array');
    }

    public function test_chgrp()
    {
        // chgrp($filename = false, $group = false);
        $this->assertFalse(
            $this->FS->chgrp(FS_TEST_LOG_LOCATION . DIRECTORY_SEPARATOR . 'nondir', getmygid()), 
            'Changing group of non existing source should return false');
        $this->assertTrue($this->FS->chgrp(FS_TEST_LOG_LOCATION, getmygid()), 
            'Changing group is not alloed for your user');
    }

    public function test_chmod()
    {
        // chmod($filename = false, $mode = false);
        $this->assertFalse($this->FS->chmod(FS_TEST_LOG_LOCATION . DIRECTORY_SEPARATOR . 'nondir', 750), 
            'Changing mode of non existing source should return false');
        $this->assertTrue($this->FS->chmod(FS_TEST_LOG_LOCATION, 750), 
            'Changing mode is not alloed for your user');
    }

    public function test_chown()
    {
        // chown($filename = false, $user = false);
        $this->assertFalse(
            $this->FS->chown(FS_TEST_LOG_LOCATION . DIRECTORY_SEPARATOR . 'nondir', getmyuid()), 
            'Changing user of non existing source should return false');
        $this->assertTrue($this->FS->chown(FS_TEST_LOG_LOCATION, getmyuid()), 
            'Changing user is not alloed for your user');
    }

    public function test_clearstatcache()
    {
        // clearstatcache()
        $this->assertNull($this->FS->clearstatcache(), 'Must always be void');
        $this->assertNull($this->FS->clearstatcache(true, '~/.HOWI/tmp/libhowi-filesystem'), 'Must always be void');
    }

    public function test_copy()
    {
        // copy($source = false, $dest = false, $context = false);
        $copy_destination = FS_TEST_LOCATION . DIRECTORY_SEPARATOR . 'archives' . DIRECTORY_SEPARATOR .
             'log-copy';
        
        $this->FS->dir('oldlogs', '~/.HOWI/tmp/libhowi-filesystem/log');
        
        $this->assertFalse($this->FS->isDir($copy_destination), 
            '~/.HOWI/tmp/libhowi-filesystem/log-copy should not exist');
        $this->assertTrue($this->FS->copy('~/.HOWI/tmp/libhowi-filesystem/log', $copy_destination));
        $this->assertTrue($this->FS->isDir($copy_destination), 
            '~/.HOWI/tmp/libhowi-filesystem/log-copy should now exist');
        
        $this->FS->rm($copy_destination);
        $context = stream_context_create(
            array(
                'socket' => array(
                    'bindto' => '0:7000'
                )
            ));
        $this->assertFalse($this->FS->isDir($copy_destination), 
            '~/.HOWI/tmp/libhowi-filesystem/log-copy should not exist');
        
        $this->assertTrue($this->FS->copy('~/.HOWI/tmp/libhowi-filesystem/log', $copy_destination, $context));
        $this->assertTrue($this->FS->isDir($copy_destination), 
            '~/.HOWI/tmp/libhowi-filesystem/log-copy should now exist');
        
        $this->assertFalse($this->FS->copy(), 'Copy without source and destination must return false');
    }

    public function test_mv()
    {
        $tobemoved = FS_TEST_LOCATION . DIRECTORY_SEPARATOR . 'archives' . DIRECTORY_SEPARATOR . 'log-copy';
        $moveto = FS_TEST_LOCATION . DIRECTORY_SEPARATOR . 'archives' . DIRECTORY_SEPARATOR . 'log-backup';
        
        $this->assertFalse($this->FS->mv());
        $this->assertFalse($this->FS->isDir($moveto));
        $this->assertTrue($this->FS->mv($tobemoved, $moveto));
        $this->assertTrue($this->FS->isDir($moveto));
    }

    public function test_namePatternMatch()
    {
        $this->assertFalse($this->FS->namePatternMatch());
        $this->assertFalse($this->FS->namePatternMatch("*lib[-]howi", "libhowi-filesystem"));
        $this->assertTrue($this->FS->namePatternMatch("*libhowi[-]filesystem", "libhowi-filesystem"));
    }

    public function test_getGlob()
    {
        $this->assertInternalType('array', $this->FS->getGlob());
    }

    public function test_isUploadedFile()
    {
        $this->assertFalse($this->FS->isUploadedFile());
        $this->assertFalse($this->FS->isUploadedFile(FS_TEST_LOCATION));
        $this->assertFalse($this->FS->isUploadedFile(FS_TEST_LOG_LOCATION));
    }

    public function test_getRealpathCache()
    {
        $this->assertInternalType('array', $this->FS->getRealpathCache());
    }

    public function test_getRealpathCacheSize()
    {
        $this->assertInternalType('int', $this->FS->getRealpathCacheSize());
    }

    public function test_close()
    {
        $this->FS->dir('log', FS_TEST_LOCATION);
        
        $this->assertArrayHasKey('log', $this->FS->getDirkeys());
        
        $this->FS->close('log');
        
        $this->assertArrayNotHasKey('log', $this->FS->getDirkeys());
    }

    public function test_touch()
    {
        // touch($filename = false, $time = false, $atime = false);
        $test_file = FS_TEST_LOCATION . DIRECTORY_SEPARATOR . 'documents' . DIRECTORY_SEPARATOR .
             'test_file.txt';
        $time = time() - 3600;
        $this->assertNotEquals($time, $this->FS->getATime(false, $test_file));
        $this->assertNotEquals($time, $this->FS->getCTime(false, $test_file));
        $this->assertNotEquals($time, $this->FS->getMTime(false, $test_file));
        
        $this->assertTrue($this->FS->touch($test_file, $time, $time));
        $this->FS->clearstatcache();
        
        $this->assertEquals($time, $this->FS->getATime(false, $test_file), 'getAccessTime');
        $this->assertEquals($time, $this->FS->getMTime(false, $test_file), 'getModifTime');
    }

    public function test_getSize()
    {
        $this->assertInternalType('int', $this->FS->getSize(false, FS_TEST_LOCATION));
        $this->assertInternalType('int', $this->FS->getSize(false, FS_TEST_LOG_LOCATION));
        $this->assertInternalType('array', $this->FS->getSize(true, FS_TEST_LOCATION));
        $this->assertInternalType('array', $this->FS->getSize(true, FS_TEST_LOG_LOCATION));
    }

    public function test_getATime()
    {
        $this->assertInternalType('int', $this->FS->getATime(false, FS_TEST_LOCATION), 
            'Default getAccessTime must be a int');
        $this->assertInternalType('string', $this->FS->getATime('ago', FS_TEST_LOCATION), 
            'getAccessTime must be a string');
        $this->assertInternalType('string', $this->FS->getATime('ago.single', FS_TEST_LOCATION), 
            'getAccessTime must be a string');
        $this->assertInternalType('string', $this->FS->getATime('c', FS_TEST_LOCATION), 
            'getAccessTime must be a string');
    }

    public function test_getCTime()
    {
        $this->assertInternalType('int', $this->FS->getCTime(false, FS_TEST_LOCATION), 
            'Default getChangeTime must be a int');
        $this->assertInternalType('string', $this->FS->getCTime('ago', FS_TEST_LOCATION), 
            ' getChangeTime must be a string');
        $this->assertInternalType('string', $this->FS->getCTime('ago.single', FS_TEST_LOCATION), 
            ' getChangeTime must be a string');
        $this->assertInternalType('string', $this->FS->getCTime('c', FS_TEST_LOCATION), 
            ' getChangeTime must be a string');
    }

    public function test_getMTime()
    {
        $this->assertInternalType('int', $this->FS->getMTime(false, FS_TEST_LOCATION), 
            'Default getModifTime must be a int');
        $this->assertInternalType('string', $this->FS->getMTime('ago', FS_TEST_LOCATION), 
            'Default getModifTime must be a string');
        $this->assertInternalType('string', $this->FS->getMTime('ago.single', FS_TEST_LOCATION), 
            'Default getModifTime must be a string');
        $this->assertInternalType('string', $this->FS->getMTime('c', FS_TEST_LOCATION), 
            'Default getModifTime must be a string');
    }

    public function test_ct_ago()
    {
        $time = time() - 86401;
        $test = $this->FS->ct_ago($time);
        $this->assertEquals('1 day ago', $test, 'Wrong respons eformat');
    }

    public function test_ct_ago2()
    {
        $time = time() - 34218061 /* year 2016*/ - 86400;
        $test = $this->FS->ct_ago($time, 'y,m,d,h,i,s', true, false, false);
        $this->assertEquals('1 year 1 month 1 day 1 hour 1 minute 1 second ago', $test, 
            'Wrong respons eformat');
        
    }

    public function test_ct_ago3()
    {
        $time = time() - 34218061 /* year 2016*/ - 86400;
        $test = $this->FS->ct_ago($time, 'y,m,d,h,i,s', false, false, false);
        $this->assertEquals('1y 1m 1d 1h 1m 1s ago', $test, 'Wrong respons eformat');
    }

    public function test_ct_ago4()
    {
        $time = time() - 34131661 /* year 2016*/ - 86400;
        $test = $this->FS->ct_ago($time, 'y,m,d,h,i,s', false, true, false);
        $this->assertEquals('1y 1m 1h 1m 1s ago', $test, 'Wrong respons eformat');
    }

    public function test_ct_ago5()
    {
        $time = time();
        $test = $this->FS->ct_ago($time, 'y,m,d,h,i,s');
        $this->assertEquals('just now', $test, 'Wrong respons eformat');
    }

    public function test_ct_ago6()
    {
        $time = time() - 31218061;
        $test = $this->FS->ct_ago($time);
        $this->assertEquals('11 months ago', $test, 'Wrong respons eformat');
    }

    public function test_openFile()
    {
        $this->assertFalse($this->FS->openFile(FS_TEST_LOG_LOCATION));
    }

    public function test_setFileClass()
    {
        $this->assertFalse($this->FS->setFileClass('SplFileObject'));
        
        $this->assertFalse($this->FS->setFileClass('\SplFileObjectNonExisting'));
        
        $this->assertFalse($this->FS->setFileClass('\HOWI3\libhowi\Filesystem\php5\Objects\FileObject'));
    }

    public function test_setInfoClass()
    {
        $this->assertFalse($this->FS->setInfoClass('SplFileInfo'));
        
        $this->assertFalse($this->FS->setInfoClass('\SplFileInfoNonExisting'));
        
        $this->assertFalse($this->FS->setInfoClass('\HOWI3\libhowi\Filesystem\php5\Objects\infoObject'));
    }

    public function test_getOwnerName()
    {
        $username = posix_getpwuid(getmyuid());
        $this->assertInternalType('string', $this->FS->getOwnerName(FS_TEST_LOCATION));
        $this->assertEquals($username['name'], $this->FS->getOwnerName(FS_TEST_LOCATION));
        $this->assertFalse($this->FS->getOwnerName());
    }

    public function test_getGroupName()
    {
        $group = posix_getgrgid(getmygid())['name'];
        
        $this->assertEquals($group, $this->FS->getGroupName(FS_TEST_LOCATION), 
            FS_TEST_LOCATION . ' group should match with current users group');
        
        $this->assertFalse($this->FS->getGroupName());
    }

    public function test_getRealpath()
    {
        $this->assertEquals(getenv("HOME") . '/tmp', 
            $this->FS->getRealpath(getenv("HOME") . '/tmp/libhowi-filesystem/../'));
    }

    public function test_getPathinfo()
    {
        
        /**
         * PATHINFO_DIRNAME, PATHINFO_BASENAME, PATHINFO_EXTENSION or PATHINFO_FILENAME.
         */
        $this->assertInternalType('array', $this->FS->getPathinfo(FS_TEST_LOG_LOCATION));
        
        $this->assertEquals(getenv("HOME") . '/.HOWI/tmp/libhowi-filesystem/log', 
            $this->FS->getPathinfo(FS_TEST_LOG_LOCATION, PATHINFO_DIRNAME), 
            '~/.HOWI/tmp/libhowi-filesystem/log/phpunitTestLog.txt PATHINFO_DIRNAME should be string ');
        
        $this->assertEquals('phpunit_test.log', 
            $this->FS->getPathinfo(FS_TEST_LOG_LOCATION, PATHINFO_BASENAME), 
            '~/.HOWI/tmp/libhowi-filesystem/log/phpunitTestLog.txt PATHINFO_DIRNAME should be string ');
        
        $this->assertEquals('log', $this->FS->getPathinfo(FS_TEST_LOG_LOCATION, PATHINFO_EXTENSION), 
            '~/.HOWI/tmp/libhowi-filesystem/log/phpunitTestLog.txt PATHINFO_DIRNAME should be string ');
        
        $this->assertEquals('phpunit_test', $this->FS->getPathinfo(FS_TEST_LOG_LOCATION, PATHINFO_FILENAME), 
            '~/.HOWI/tmp/libhowi-filesystem/log/phpunitTestLog.txt PATHINFO_DIRNAME should be string ');
    }

    public function test_getGroup()
    {
        $group = getmygid();
        
        $this->assertEquals($group, $this->FS->getGroup(FS_TEST_LOCATION), 
            '~/.HOWI/tmp/libhowi-filesystem group should match with current users group');
        $this->assertFalse($this->FS->getGroup());
    }

    public function test_getOwner()
    {
        $userID = getmyuid();
        $this->assertInternalType('int', $this->FS->getOwner(FS_TEST_LOCATION));
        $this->assertEquals($userID, $this->FS->getOwner(FS_TEST_LOCATION), 
            '~/.HOWI/tmp/libhowi-filesystem owner should match with current users ID');
        $this->assertFalse($this->FS->getOwner());
    }

    public function test_getInode()
    {
        $this->assertInternalType('int', $this->FS->getInode(FS_TEST_LOCATION));
    }

    public function test_getBasename()
    {
        $this->assertEquals('', $this->FS->getBasename(''), 
            'getBasename with null or empty string should return empty string');
        $this->assertEquals('libhowi-filesystem', $this->FS->getBasename(false, '/tmp/libhowi-filesystem'));
        $this->assertEquals('libhowi-filesystem', $this->FS->getBasename(false, FS_TEST_LOCATION));
        $this->assertEquals('phpunit_test.log', $this->FS->getBasename(false, FS_TEST_LOG_LOCATION));
        $this->assertEquals('phpunit_test', $this->FS->getBasename('.log', FS_TEST_LOG_LOCATION));
    }

    public function test_getPerms()
    {
        $this->assertEquals(750, $this->FS->getPerms(FS_TEST_LOCATION), 
            '~/.HOWI/tmp/libhowi-filesystem permissions should be 0750');
    }

    public function test_getType()
    {
        $file = $this->FS->getType(FS_TEST_LOG_LOCATION);
        $dir = $this->FS->getType(FS_TEST_LOCATION);
        $this->assertInternalType('string', $file);
        $this->assertInternalType('string', $dir);
        $this->assertEquals('file', $file);
        $this->assertEquals('dir', $dir);
    }

    public function test_isDot()
    {
        $this->assertFalse($this->FS->isDot());
    }

    public function test_isDir()
    {
        $this->assertFalse($this->FS->isDir(FS_TEST_LOG_LOCATION), 
            '~/.HOWI/tmp/libhowi-filesystem/log/phpunitTestLog.txt');
        $this->assertTrue($this->FS->isDir(FS_TEST_LOCATION), '~/.HOWI/tmp/libhowi-filesystem');
    }

    public function test_isExecutable()
    {
        $this->assertFalse($this->FS->isExecutable());
        $this->assertTrue($this->FS->isExecutable(FS_TEST_LOG_LOCATION));
        $this->assertTrue($this->FS->isExecutable('test'), 'test');
        $this->assertTrue($this->FS->isExecutable(FS_TEST_LOCATION));
    }

    public function test_isFile()
    {
        $this->assertFalse($this->FS->isFile());
        $this->assertFalse($this->FS->isFile(FS_TEST_LOCATION));
        $this->assertTrue($this->FS->isFile('test'), 'test');
        $this->assertTrue($this->FS->isFile(FS_TEST_LOG_LOCATION));
    }

    public function test_isLink()
    {
        $this->assertFalse($this->FS->isLink());
        $this->assertFalse($this->FS->isLink(FS_TEST_LOCATION));
        $this->assertFalse($this->FS->isLink(FS_TEST_LOG_LOCATION));
        $this->assertTrue(
            $this->FS->isLink(
                FS_TEST_LOCATION . DIRECTORY_SEPARATOR . 'documents' . DIRECTORY_SEPARATOR .
                     'test_file_symlink.txt'));
    }

    public function test_exists()
    {
        $this->assertFalse($this->FS->exists(), 'exists with empty or null must return false');
        $this->assertTrue($this->FS->exists('/etc'), 'etc dir should exist');
        $this->assertFalse($this->FS->exists('/nodir_rand'), 'nodir_rand should not exist');
        $this->assertTrue($this->FS->exists(FS_TEST_LOG_LOCATION));
        $this->assertTrue($this->FS->exists(FS_TEST_LOCATION));
    }

    public function test_getFilename()
    {
        $this->assertEquals('phpunit_test.log', $this->FS->getFilename(FS_TEST_LOG_LOCATION));
    }

    public function test_getExtension()
    {
        $this->assertEquals('log', $this->FS->getExtension(FS_TEST_LOG_LOCATION));
    }

    public function test_getPath()
    {
        $this->assertInternalType('string', $this->FS->getPath());
    }

    public function test_getPathname()
    {
        $this->assertEquals($this->FS->getCwd(), $this->FS->getPathname());
    }

    public function test_createStructure()
    {
        $test_structure = array(
            'documents' => array(
                'archives' => array(),
                '0' => 'test_file.txt'
            ),
            'videos' => array(),
            'images' => array(
                'photos' => array(),
                'raw' => array()
            )
        )
        ;
        $rootpath = FS_TEST_LOCATION . DIRECTORY_SEPARATOR . 'structure_test';
        
        $this->assertFalse($this->FS->createStructure('/', $test_structure));
        $this->assertEquals(506, $this->FS->getCode());
        
        $this->assertTrue($this->FS->createStructure($rootpath, $test_structure));
        $this->assertEquals(704, $this->FS->getCode());
        
        $this->assertTrue($this->FS->exists($rootpath . DIRECTORY_SEPARATOR . 'documents'));
        $this->assertTrue($this->FS->exists($rootpath . DIRECTORY_SEPARATOR . 'videos'));
        $this->assertTrue($this->FS->exists($rootpath . DIRECTORY_SEPARATOR . 'images'));
    }

    public function test_createStructure2()
    {
        $test_structure = $this->FS->dir('libhowi-filesystem','~/.HOWI/tmp')->lsTree();
        
        $rootpath = FS_TEST_LOCATION . DIRECTORY_SEPARATOR . 'structure_test';
    
        $this->assertFalse($this->FS->createStructure('/', $test_structure));
        $this->assertEquals(506, $this->FS->getCode());
    
        $this->assertTrue($this->FS->createStructure($rootpath, $test_structure));
        $this->assertEquals(704, $this->FS->getCode());
    
        $this->assertTrue($this->FS->exists($rootpath . DIRECTORY_SEPARATOR . 'archives'));
        $this->assertTrue($this->FS->exists($rootpath . DIRECTORY_SEPARATOR . 'documents'));
        $this->assertTrue($this->FS->exists($rootpath . DIRECTORY_SEPARATOR . 'images'));
        $this->assertTrue($this->FS->exists($rootpath . DIRECTORY_SEPARATOR . 'log'));
        $this->assertTrue($this->FS->exists($rootpath . DIRECTORY_SEPARATOR . 'structure_test'));
        $this->assertTrue($this->FS->exists($rootpath . DIRECTORY_SEPARATOR . 'tmp'));
        $this->assertTrue($this->FS->exists($rootpath . DIRECTORY_SEPARATOR . 'videos'));
    }
    
    public function test_functions()
    {
        $methods = get_class_methods($this->FS);
        
        foreach ($methods as $method) {
            $skip = array(
                '__construct', // Tested by load
                'dir', // Tested by directory tests
                'file', // Tested by file tests
                'infoObject', // tested by info tests
                'tmp', // Testes by tmp tests
                'link', // Tested by link tests
                'setLogLevel', // Teste by $this->test_set_get_LogLevel()
                'getLogLevel', // --"--
                'setLogFile', // Tested by $this->test_set_get_LogFile()
                'getLogFile', // --"--
                'getUID', // Tested by $this->test_get_set_UID()
                'setUID', // --"--
                'getUsername', // Tested by $this->test_set_get_Username()
                'setUsername', // --"--
                'getStatus', // tested by $this->test_get_set_Status()
                'setStatus', // --"--
                'getCode', // tested by $this->test_get_set_Code()
                'setCode', // --"--
                'getDirkeys'
            ); // Tested by $this->test_close()
               // --"--
            
            if (! in_array($method, $skip) && ! method_exists($this, "test_$method") && empty($fails)) {
                $this->addWarning(
                    "Method $method has no test_$method! Implement test or add method into 'skip' array");
                break;
            }
        }
        $this->addToAssertionCount(1);
    }

    protected function tearDown()
    {
        unset($this->FS);
        unset($_FILES);
    }
}
