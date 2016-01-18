<?php 
use HOWI3\libhowi\Filesystem\FS;

class InitTest extends PHPUnit_Framework_TestCase
{
    protected $FS;
    
    public function setUp()
    {
        if($this->FS instanceof HOWI3\libhowi\Filesystem\FS)
            return;
        
        $this->FS = FS::load();
        $logfile = $this->FS->makeAbsolute(FS_TEST_LOG_LOCATION);
        if(!file_exists($logfile) && is_dir(dirname($logfile)))
            touch($logfile);
        
        $this->FS->setLogLevel(FS_TEST_LOG_LEVEL);
        $this->FS->setUID(getmyuid());
        $this->FS->setUsername(get_current_user());
        $this->FS->setLogFile($logfile);
        
    }
    
    public function test_response()
    {
        $this->FS->critical(3);
        $this->assertEquals(get_current_user(),$this->FS->response()->getUsername());
        $this->assertEquals(getmyuid(),$this->FS->response()->getUID());
        $this->assertEquals('CRITICAL',$this->FS->response()->getLevel());
        $this->assertFalse($this->FS->response()->getContext());
        
    }
    
    public function test_libhowi_autoloader()
    {
        $this->assertNull(libhowi_autoloder(), "Autoloader must not return anything!");
    }
    
    public function test_php_version_to_low()
    {
        $this->assertGreaterThanOrEqual(50400, PHP_VERSION_ID,
            "No point to test it with PHP version less than PHP version 5.4!");
        $this->assertEquals(100, FS::load(false,50399)->getCode());
    }
    
    public function test_prepare_test_destination()
    {
        
        
        $HOME = getenv("HOME");
        $FS_HOME = $this->FS->makeAbsolute('~/');
        
        $this->assertEquals($HOME . '/', $FS_HOME,
            'Could not verify users home directory, that might be a bug');
        $is_writable = $this->FS->isWritable($FS_HOME . '/tmp');
        
        $this->assertTrue($is_writable, 'Could not dedect is tmp directory writable at users home');
        
        $TEST_DIR = $this->FS->makeAbsolute(FS_TEST_LOCATION);
        
        if ($is_writable) {
            if ($this->FS->isDir($TEST_DIR)) {
                
                $this->FS->rm(FS_TEST_LOCATION);
                $this->assertTrue($this->FS->getStatus());
                $this->assertEquals(702,$this->FS->getCode());
            }
            $this->FS->dir('libhowi-filesystem', '~/HOWI3/tmp');
            $this->assertTrue($this->FS->getStatus(), 'Failed to create test directory');
            
            $this->FS->dir('log', FS_TEST_LOCATION);
            $this->assertTrue($this->FS->getStatus(), 'Failed to create log directory');
            $this->assertTrue($this->FS->isDir('~/HOWI3/tmp/libhowi-filesystem/log'));
            
            $this->FS->debug(8, 'Starting phpunit tests');
            
        }
    }
    
    public function test_FS_instance()
    {
        $this->assertInstanceOf('HOWI3\libhowi\Filesystem\Commons\FilesystemInterface', FS::load());
    }
    
    public function test_FS_load_with_non_existing_destination()
    {
        $FS = FS::load('../lets/try/to/go/some/randome/place/which/do/not/exist');
        $this->assertInstanceOf('HOWI3\libhowi\Filesystem\Commons\FilesystemInterface', $FS);
        $this->assertEquals(500, $FS->getCode(),
            "Starting Code must be int 500 if invalid destination is provided!");
    }
    
    public function test_FS_load_with_true()
    {
        $FS = FS::load(true);
        $this->assertInstanceOf('HOWI3\libhowi\Filesystem\Commons\FilesystemInterface', $FS);
        $this->assertEquals(801, $FS->getCode(),
            "Starting Code must be 0 if true is provided the cwd will be directory containing the calling script!");
    }
    
    protected function tearDown()
    {
        unset($this->FS);
    }
}