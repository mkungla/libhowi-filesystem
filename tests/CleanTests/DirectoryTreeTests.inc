<?php
use HOWI3\libhowi\Filesystem\FS;


class DirectoryTreeTests extends PHPUnit_Framework_TestCase
{

    protected $FS;

    protected $previous = null;

    public function setUp()
    {
        if($this->FS instanceof HOWI3\libhowi\Filesystem\FS)
            return;
        
        $this->FS = FS::load();
        $logfile = $this->FS->makeAbsolute(FS_TEST_LOG_LOCATION);
        $this->FS->setLogLevel(FS_TEST_LOG_LEVEL);
        $this->FS->setUID(getmyuid());
        $this->FS->setUsername(get_current_user());
        $this->FS->setLogFile($logfile);
        /*
         * if ($incomplete)
         * $this->markTestIncomplete();
         * if ($skipped)
         * $this->markTestSkipped();
         */
    }

    protected function addWarning($msg, Exception $previous = null)
    {
        $add_warning = $this->getTestResultObject();
        $msg = new PHPUnit_Framework_Warning($msg, 0, $previous);
        $add_warning->addWarning($this, $msg, time());
        $this->setTestResultObject($add_warning);
    }

    protected function addError($msg, Exception $previous = null)
    {
        $add_error = $this->getTestResultObject();
        $msg = new PHPUnit_Framework_AssertionFailedError($msg, 0, $previous);
        $add_error->addError($this, $msg, time());
        $this->setTestResultObject($add_error);
    }

    protected function addFailure($msg, Exception $previous = null)
    {
        $add_failure = $this->getTestResultObject();
        $msg = new PHPUnit_Framework_AssertionFailedError($msg, 0, $previous);
        $add_failure->addFailure($this, $msg, time());
        $this->setTestResultObject($add_failure);
    }

    public function test_c()
    {
        
        
        $this->assertInstanceOf('HOWI3\libhowi\Filesystem\Commons\ObjectInterfaces\DirectoryTreeInterface', 
            $this->FS->dir('libhowi-filesystem', '~/HOWI3/tmp')
                ->c('log'));
        $this->assertFalse($this->FS->dir('libhowi-filesystem', '~/HOWI3/tmp')
            ->c('logs'), 'logs does not exist');
    }

    public function test_hasChildren()
    {
        
        $this->FS->dir('libhowi-filesystem', '~/HOWI3/tmp');
        $this->FS->dir('archives', FS_TEST_LOCATION);
        $this->FS->dir('log', FS_TEST_LOCATION);
        
        $this->assertInternalType('boolean', $this->FS->dir('libhowi-filesystem')
            ->hasChildren());
        
        /* Check children on dot! */
        $this->assertTrue($this->FS->dir('libhowi-filesystem')
            ->hasChildren(), "libhowi-filesystem has children");
        
        $this->assertFalse($this->FS->dir('archives')
            ->hasChildren(), 'archives do not have children');
        
        /* Check children no items */
        $this->assertTrue($this->FS->dir('log')
            ->hasChildren(), 'log has children');
        
        $this->assertEquals('log', $this->FS->dir('libhowi-filesystem')
            ->getBasename());
        
        /* Our pointer should be on videos dir after seek(3) | next */
        $this->FS->dir('libhowi-filesystem')->next();
        
        $this->assertEquals('videos', $this->FS->dir('libhowi-filesystem')
            ->getBasename());
        $this->assertTrue($this->FS->dir('libhowi-filesystem')
            ->hasChildren(), 
            "We are looking libhowi-filesystem which has children what ever point the pointer is");
        
        /**
         * If we are attending to check does libhowi-filesystem children have childrens their own
         * then we call it as follows with c()
         */
        $this->assertTrue(
            $this->FS->dir('libhowi-filesystem')
                ->c('log')
                ->hasChildren(), 'log has children');
        
        $this->assertFalse(
            $this->FS->dir('libhowi-filesystem')
                ->c('archives')
                ->hasChildren(), 'archives do not have children');
    }

    public function test_getChildren()
    {
        
        
        $this->FS->dir('libhowi-filesystem', '~/HOWI3/tmp');
        
        /* Move pointer to log drectory */
        $this->FS->dir('libhowi-filesystem')->seek(2);
        
        /* Behaves same as c($basename) but you call it on pointer instead */
        $this->assertInstanceOf('HOWI3\libhowi\Filesystem\Commons\ObjectInterfaces\DirectoryTreeInterface', 
            $this->FS->dir('libhowi-filesystem')
                ->getChildren());
        
        $from_c = $this->FS->dir('libhowi-filesystem', '~/HOWI3/tmp')
            ->c('log')
            ->getBasename();
       
        $this->assertEquals($from_c, 'phpunit_test.log');
    }

    public function test_getSubPath()
    {
        
        
        $this->FS->dir('libhowi-filesystem', '~/HOWI3/tmp');
        $this->assertInternalType('string', $this->FS->dir('libhowi-filesystem')
            ->getSubPath());
        
        $this->assertEquals('', 
            $this->FS->dir('libhowi-filesystem')
                ->c('log')
                ->getSubPath());
    }

    public function test_getSubPathname()
    {
        
        
        $this->FS->dir('libhowi-filesystem', '~/HOWI3/tmp');
        $this->assertInternalType('string', $this->FS->dir('libhowi-filesystem')
            ->getSubPath());
        
        $this->assertEquals('phpunit_test.log', 
            $this->FS->dir('libhowi-filesystem')
                ->c('log')
                ->getSubPathname());
    }

    public function test_current()
    {
        
        $this->FS->dir('libhowi-filesystem', '~/HOWI3/tmp');
        
        $this->assertInstanceOf('HOWI3\libhowi\Filesystem\Commons\ObjectInterfaces\InfoInterface', 
            $this->FS->dir('libhowi-filesystem')
                ->current());
        
        $this->assertInstanceOf('HOWI3\libhowi\Filesystem\Commons\ObjectInterfaces\InfoInterface', 
            $this->FS->dir('libhowi-filesystem')
                ->c('log')
                ->current());
    }

    public function test_key()
    {
        
        $this->FS->dir('libhowi-filesystem', '~/HOWI3/tmp');
        $this->assertInternalType('string', $this->FS->dir('libhowi-filesystem')
            ->key());
    }

    public function test_getFlags()
    {
        
        $this->FS->dir('libhowi-filesystem', '~/HOWI3/tmp');
        $this->assertInternalType('int', $this->FS->dir('libhowi-filesystem')
            ->getFlags());
    }

    public function test_setFlags()
    {
            //
        $this->FS->dir('libhowi-filesystem', '~/HOWI3/tmp');
        if(PHP_VERSION_ID >= 70000)
        {
            $this->assertNull(
                $this->FS->dir('libhowi-filesystem')
                ->setFlags(\HOWI3\libhowi\Filesystem\php7\Objects\DirectoryTreeObject::SKIP_DOTS));
        }
        else {
            $this->assertNull(
                $this->FS->dir('libhowi-filesystem')
                ->setFlags(\HOWI3\libhowi\Filesystem\php5\Objects\DirectoryTreeObject::SKIP_DOTS));
        }
        
    }

    public function test_getFilename()
    {
        
        $this->FS->dir('libhowi-filesystem', '~/HOWI3/tmp');
        
        $this->assertEquals('phpunit_test.log', 
            $this->FS->dir('libhowi-filesystem')
                ->c('log')
                ->getFilename());
    }

    public function test_getExtension()
    {
        
        $this->FS->dir('libhowi-filesystem', '~/HOWI3/tmp');
        
        $this->assertEquals('log', 
            $this->FS->dir('libhowi-filesystem')
                ->c('log')
                ->getExtension());
    }

    public function test_getBasename()
    {
        
        $this->FS->dir('libhowi-filesystem', '~/HOWI3/tmp');
        
        $this->assertEquals('phpunit_test.log', 
            $this->FS->dir('libhowi-filesystem')
                ->c('log')
                ->getBasename());
        
        $this->assertEquals('phpunit_test', 
            $this->FS->dir('libhowi-filesystem')
                ->c('log')
                ->getBasename('.log'));
        
        /* Use it from any context with path */
        $this->assertEquals('libhowi-filesystem', 
            $this->FS->dir('libhowi-filesystem')
                ->c('log')
                ->getBasename(false, FS_TEST_LOCATION));
    }

    public function test___toString()
    {
        
        $this->FS->dir('libhowi-filesystem', '~/HOWI3/tmp');
        
        $this->assertEquals($this->FS->dir('libhowi-filesystem')
            ->getFilename(), $this->FS->dir('libhowi-filesystem')
            ->__toString());
    }

    public function test_getPath()
    {
        
        $this->FS->dir('libhowi-filesystem', '~/HOWI3/tmp');
        $this->assertEquals(getenv('HOME') . '/HOWI3/tmp/libhowi-filesystem/log', 
            $this->FS->dir('libhowi-filesystem')
                ->c('log')
                ->getPath(), 'Failed to assert iterator getPath');
    }

    public function test_getPathName()
    {
        
        $this->FS->dir('libhowi-filesystem', '~/HOWI3/tmp');
        
        $this->assertEquals(getenv('HOME') . '/HOWI3/tmp/libhowi-filesystem/log/phpunit_test.log', 
            $this->FS->dir('libhowi-filesystem')
                ->c('log')
                ->getPathName(), 'Failed to assert iterator getPath');
    }

    public function test_getPerms()
    {
        
        $this->FS->dir('libhowi-filesystem', '~/HOWI3/tmp');
        
        $this->assertEquals(750, $this->FS->dir('libhowi-filesystem')
            ->getPerms(), '~/HOWI3/tmp/libhowi-filesystem permissions should be 0755');
    }

    public function test_getInode()
    {
        
        $this->FS->dir('libhowi-filesystem', '~/HOWI3/tmp');
        
        $this->assertInternalType('int', $this->FS->dir('libhowi-filesystem')
            ->getInode());
    }

    public function test_getSize()
    {
        
        $this->FS->dir('libhowi-filesystem', '~/HOWI3/tmp');
        
        $this->assertInternalType('int', $this->FS->dir('libhowi-filesystem')
            ->getSize());
        $this->assertInternalType('array', $this->FS->dir('libhowi-filesystem')
            ->getSize(true));
        
        $this->assertInternalType('int', 
            $this->FS->dir('libhowi-filesystem')
                ->c('log')
                ->getSize());
        $this->assertInternalType('array', 
            $this->FS->dir('libhowi-filesystem')
                ->c('log')
                ->getSize(true));
    }

    public function test_getOwner()
    {
        
        $this->FS->dir('libhowi-filesystem', '~/HOWI3/tmp');
        
        $userID = getmyuid();
        $this->assertInternalType('int', $this->FS->dir('libhowi-filesystem')
            ->getOwner());
        $this->assertEquals($userID, $this->FS->dir('libhowi-filesystem')
            ->getOwner(), '~/HOWI3/tmp/libhowi-filesystem owner should match with current users ID');
    }

    public function test_getGroup()
    {
        // filegroup($filename = false);
        
        $this->FS->dir('libhowi-filesystem', '~/HOWI3/tmp');
        $group = getmygid();
        if($group) {
            $this->assertEquals($group, $this->FS->dir('libhowi-filesystem')
                ->getGroup(), '~/HOWI3/tmp/libhowi-filesystem group should match with current users group');
        } else {
            $this->markTestSkipped();
        }
        
        $this->assertEquals($group, $this->FS->dir('libhowi-filesystem')
            ->getGroup(), '~/HOWI3/tmp/libhowi-filesystem group should match with current users group');
    }

    public function test_getATime()
    {
        
        $this->FS->dir('libhowi-filesystem', '~/HOWI3/tmp');
        
        $this->assertInternalType('int', $this->FS->dir('libhowi-filesystem')
            ->getATime(), 'Default getAccessTime must be a int');
        $this->assertInternalType('string', 
            $this->FS->dir('libhowi-filesystem')
                ->getATime('ago'));
        $this->assertInternalType('string', 
            $this->FS->dir('libhowi-filesystem')
                ->getATime('ago.single'));
        $this->assertInternalType('string', $this->FS->dir('libhowi-filesystem')
            ->getATime('c'));
    }

    public function test_getMTime()
    {
        
        $this->FS->dir('libhowi-filesystem', '~/HOWI3/tmp');
        
        $this->assertInternalType('int', $this->FS->dir('libhowi-filesystem')
            ->getMTime(), 'Default getModifTime must be a int');
        $this->assertInternalType('string', 
            $this->FS->dir('libhowi-filesystem')
                ->getMTime('ago'), 'Default getModifTime must be a string');
        $this->assertInternalType('string', 
            $this->FS->dir('libhowi-filesystem')
                ->getMTime('ago.single'), 'Default getModifTime must be a string');
        $this->assertInternalType('string', $this->FS->dir('libhowi-filesystem')
            ->getMTime('c'), 'Default getModifTime must be a string');
    }

    public function test_getCTime()
    {
        
        $this->FS->dir('libhowi-filesystem', '~/HOWI3/tmp');
        
        $this->assertInternalType('int', $this->FS->dir('libhowi-filesystem')
            ->getCTime(), 'Default getAccessTime must be a int');
        $this->assertInternalType('string', 
            $this->FS->dir('libhowi-filesystem')
                ->getCTime('ago'), 'Default getAccessTime must be a int');
        $this->assertInternalType('string', 
            $this->FS->dir('libhowi-filesystem')
                ->getCTime('ago.single'), 'Default getAccessTime must be a int');
        $this->assertInternalType('string', $this->FS->dir('libhowi-filesystem')
            ->getCTime('c'), 'Default getAccessTime must be a int');
    }

    /**
     * Get the type of a variable
     *
     * @return string type
     */
    public function test_getType()
    {
        
        $this->FS->dir('libhowi-filesystem', '~/HOWI3/tmp');
        
        $this->assertInternalType('string', 
            $this->FS->dir('libhowi-filesystem')
                ->c('log')
                ->getType());
        $this->assertEquals('dir', $this->FS->dir('libhowi-filesystem')
            ->getType());
        
        $this->assertInternalType('string', 
            $this->FS->dir('libhowi-filesystem')
                ->c('log')
                ->getType());
        $this->assertEquals('file', 
            $this->FS->dir('libhowi-filesystem')
                ->c('log')
                ->getType());
    }

    public function test_isWritable()
    {
        
        $this->FS->dir('libhowi-filesystem', '~/HOWI3/tmp');
        
        $this->assertTrue($this->FS->dir('libhowi-filesystem')
            ->isWritable());
    }

    public function test_isReadable()
    {
        
        $this->FS->dir('libhowi-filesystem', '~/HOWI3/tmp');
        
        $this->assertTrue($this->FS->dir('libhowi-filesystem')
            ->isReadable());
    }

    public function test_isExecutable()
    {
        
        $this->FS->dir('libhowi-filesystem', '~/HOWI3/tmp');
        
        $this->assertFalse(
            $this->FS->dir('libhowi-filesystem')
                ->c('log')
                ->isExecutable());
        
        $this->assertTrue($this->FS->dir('libhowi-filesystem')
            ->isExecutable());
    }

    public function test_isFile()
    {
        
        $this->FS->dir('libhowi-filesystem', '~/HOWI3/tmp');
        
        $this->assertFalse($this->FS->dir('libhowi-filesystem')
            ->isFile());
        
        $this->assertTrue($this->FS->dir('libhowi-filesystem')
            ->c('log')
            ->isFile());
    }

    public function test_isDir()
    {
        
        $this->FS->dir('libhowi-filesystem', '~/HOWI3/tmp');
        
        $this->assertTrue($this->FS->dir('libhowi-filesystem')
            ->isDir());
        
        $this->assertFalse($this->FS->dir('libhowi-filesystem')
            ->c('log')
            ->isDir());
    }

    public function test_isLink()
    {
        
        $this->FS->dir('libhowi-filesystem', '~/HOWI3/tmp');
        
        $this->assertFalse($this->FS->dir('libhowi-filesystem')
            ->isLink());
    }

    public function test_getRealpath()
    {
        
        $this->FS->dir('libhowi-filesystem', '~/HOWI3/tmp');
        $this->assertInternalType('string', $this->FS->dir('libhowi-filesystem')
            ->getRealpath());
    }

    public function test_getFileInfo()
    {
        
        $this->FS->dir('libhowi-filesystem', '~/HOWI3/tmp');
        
        $this->assertInstanceOf('HOWI3\libhowi\Filesystem\Commons\ObjectInterfaces\InfoInterface', 
            $this->FS->dir('libhowi-filesystem')
                ->getFileInfo());
    }

    public function test_getPathInfo()
    {
        
        $this->FS->dir('libhowi-filesystem', '~/HOWI3/tmp');
        
        $this->assertInstanceOf('HOWI3\libhowi\Filesystem\Commons\ObjectInterfaces\InfoInterface', 
            $this->FS->dir('libhowi-filesystem')
                ->getPathInfo());
    }

    public function test_openFile()
    {
        
        $this->FS->dir('libhowi-filesystem', '~/HOWI3/tmp');
        $this->assertFalse($this->FS->dir('libhowi-filesystem')
            ->openFile());
        
        $this->assertInstanceOf('HOWI3\libhowi\Filesystem\Commons\ObjectInterfaces\FileInterface', 
            $this->FS->dir('libhowi-filesystem')
                ->c('log')
                ->openFile());
    }

    public function test_setFileClass()
    {
        
        $this->FS->dir('libhowi-filesystem', '~/HOWI3/tmp');
        
        $this->assertTrue(
            $this->FS->dir('libhowi-filesystem')
                ->c('log')
                ->current()
                ->setFileClass('SplFileObject'));
        
        $this->assertFalse(
            $this->FS->dir('libhowi-filesystem')
                ->c('log')
                ->current()
                ->setFileClass('\SplFileObjectNonExisting'));
        
       
        if(PHP_VERSION_ID >= 70000)
        {
            $this->assertTrue(
            $this->FS->dir('libhowi-filesystem')
                ->c('log')
                ->current()
                ->setFileClass('\HOWI3\libhowi\Filesystem\php7\Objects\FileObject'));
        }
        else {
             $this->assertTrue(
            $this->FS->dir('libhowi-filesystem')
                ->c('log')
                ->current()
                ->setFileClass('\HOWI3\libhowi\Filesystem\php5\Objects\FileObject'));
        }
    }

    public function test_setInfoClass()
    {
        
        $this->FS->dir('libhowi-filesystem', '~/HOWI3/tmp');
        
        $this->assertTrue(
            $this->FS->dir('libhowi-filesystem')
                ->c('log')
                ->current()
                ->setInfoClass('SplFileInfo'));
        
        $this->assertFalse(
            $this->FS->dir('libhowi-filesystem')
                ->c('log')
                ->current()
                ->setInfoClass('\SplFileInfoNonExisting'));
        
        if(PHP_VERSION_ID >= 70000)
        {
            $this->assertTrue(
            $this->FS->dir('libhowi-filesystem')
                ->c('log')
                ->current()
                ->setInfoClass('\HOWI3\libhowi\Filesystem\php7\Objects\InfoObject'));
        }
        else {
            $this->assertTrue(
            $this->FS->dir('libhowi-filesystem')
                ->c('log')
                ->current()
                ->setInfoClass('\HOWI3\libhowi\Filesystem\php5\Objects\InfoObject'));
        }
        
    }

    public function test_getCwd()
    {
        
        $this->FS->dir('libhowi-filesystem', '~/HOWI3/tmp');
        $this->assertEquals(dirname(__DIR__), $this->FS->dir('libhowi-filesystem')
            ->getCwd());
    }

    public function test_ls()
    {
        
        $this->FS->dir('libhowi-filesystem', '~/HOWI3/tmp');
        
       
        /**
         * tmp link
         */
        $target = $this->FS->makeAbsolute('~/HOWI3/tmp/libhowi-filesystem/videos');
        $link = $this->FS->makeAbsolute('~/HOWI3/tmp/libhowi-filesystem/videos-link');
        symlink($target, $link);
        
        $this->assertInternalType('array', $this->FS->dir('libhowi-filesystem')
            ->ls());
        
        $this->assertInternalType('array', 
            $this->FS->dir('libhowi-filesystem')
                ->c('log')
                ->ls());
        
        $this->assertInternalType('array', $this->FS->dir('libhowi-filesystem')
            ->ls(true));
        
        $this->assertInternalType('array', 
            $this->FS->dir('libhowi-filesystem')
                ->c('log')
                ->ls(true));
        
        unlink($link);
    }

    public function test_lsInfo()
    {
        /**
         * tmp link
         */
        $target = $this->FS->makeAbsolute('~/HOWI3/tmp/libhowi-filesystem/videos');
        $link = $this->FS->makeAbsolute('~/HOWI3/tmp/libhowi-filesystem/videos-link');
        symlink($target, $link);
        
        $this->FS->dir('libhowi-filesystem', '~/HOWI3/tmp');
        
        $this->assertInternalType('array', $this->FS->dir('libhowi-filesystem')
            ->lsInfo());
        
        $this->assertInternalType('array', 
            $this->FS->dir('libhowi-filesystem')
                ->c('log')
                ->lsInfo());
        
        $this->assertInternalType('array', $this->FS->dir('libhowi-filesystem')
            ->lsInfo(true));
        
        $this->assertInternalType('array', 
            $this->FS->dir('libhowi-filesystem')
                ->c('log')
                ->lsInfo(true));
        
        unlink($link);
    }

    public function test_lsTree()
    {
        $target = $this->FS->makeAbsolute('~/HOWI3/tmp/libhowi-filesystem/videos');
        $link = $this->FS->makeAbsolute('~/HOWI3/tmp/libhowi-filesystem/videos-link');
        symlink($target, $link);
        
        $this->FS->dir('libhowi-filesystem', '~/HOWI3/tmp');
                
        $this->assertInternalType('array', $this->FS->dir('libhowi-filesystem')
            ->lsTree());
        
        unlink($link);
    }
    
    public function test_getOwnerName()
    {
        
        $this->FS->dir('libhowi-filesystem', '~/HOWI3/tmp');
        
        $username = posix_getpwuid(getmyuid())['name'];
        $this->assertInternalType('string', $this->FS->dir('libhowi-filesystem')
            ->getOwnerName());
        $this->assertEquals($username, $this->FS->dir('libhowi-filesystem')
            ->getOwnerName(), '~/HOWI3/tmp/libhowi-filesystem owners name should match with current users name');
    }
    
    public function test_getGroupName()
    {
        
        $this->FS->dir('libhowi-filesystem', '~/HOWI3/tmp');
        
        $group = posix_getgrgid(getmygid())['name'];
    
        $this->assertEquals($group, $this->FS->dir('libhowi-filesystem')
            ->getGroupName(), '~/HOWI3/tmp/libhowi-filesystem group should match with current users group');
    }
    
    public function test_isDot()
    {
        
        $this->FS->dir('libhowi-filesystem', '~/HOWI3/tmp');
        $this->assertFalse($this->FS->dir('libhowi-filesystem')
            ->isDot());
    }
    
    public function test_functions()
    {
        
        $this->FS->dir('libhowi-filesystem', '~/HOWI3/tmp');
        
        $methods = get_class_methods($this->FS->dir('libhowi-filesystem', '~/HOWI3/tmp'));
        
        foreach ($methods as $method) {
            $skip = array(
                '__construct',
                'rewind',
                'next',
                'valid',
                'seek',
                '_bad_state_ex',
                'setLogLevel',
                'getLogLevel',
                'setLogFile',
                'getLogFile',
                'getUID',
                'getTime',
                'getUsername',
                'response',
                'getStatus',
                'getMsg',
                'getCode',
                'setUID',
                'setUsername',
                'setStatus',
                'setCode',
                'emergency',
                'alert',
                'critical',
                'error',
                'warning',
                'notice',
                'info',
                'debug',
                'makeAbsolute',
                'exists',
                'rm',
                'isAbsolute',
                'isRelative',
                'ct_ago',
                'getLinkTarget',
                'cd'
            );
            if (! in_array($method, $skip) && ! method_exists($this, "test_$method")) {
                $this->addWarning(
                    "Method $method has no test_$method! Implement test or add method into 'skip' array");
                
                break;
            }
        }
        $this->assertNull(null);
    }
    
    protected function tearDown()
    {
        unset($this->FS);
    }
}