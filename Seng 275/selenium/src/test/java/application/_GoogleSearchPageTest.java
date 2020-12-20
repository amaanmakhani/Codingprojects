package application;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.openqa.selenium.By;
import org.openqa.selenium.WebDriver;
import org.openqa.selenium.WebElement;
import org.openqa.selenium.chrome.ChromeDriver;

import java.util.List;
import java.util.Optional;

public class _GoogleSearchPageTest {

    WebDriver driver;

    @Before
    public void setUp(){
        System.setProperty("webdriver.chrome.driver", "C:\\Users\\Amaan\\Desktop\\chromedriver.exe");
        driver = new ChromeDriver();
    }

    @Test
    public void test() throws  Exception{
        driver.navigate().to("http://www.google.com");

        driver.manage().window().maximize();
        WebElement search_input = driver.findElement(By.className("gLFyf"));

        search_input.sendKeys("University of Victoria");
        //WebElement search_button = driver.findElement(By.cssSelector("input.submit[name = 'btnK']"));
        WebElement search_button = driver.findElement(By.className("gNO89b"));
        search_button.submit();
        //Thread.sleep(3000);
        WebElement nav_bar = driver.findElement(By.xpath("//div[@id='hdtb-msb-vis']"));
        List<WebElement> nav_elements = nav_bar.findElements(By.className("hdtb-mitem"));

        Optional<WebElement> images_div = nav_elements.stream().filter(ele -> "Images".equals(ele.getText())).findAny();
//        if(images_div.isPresent()){
//            Thread.sleep(9000);
//            images_div.get().findElement(By.className("qs")).click();
//
//        }
        //image_nav.click();

        Thread.sleep(10000);
    }
    

    @After
    public void tearDown(){
        System.out.println("");
        if(driver!=null){
            driver.close();
            driver.quit();
        }
    }

}
