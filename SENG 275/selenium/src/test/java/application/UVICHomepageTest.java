package application;
import org.junit.After;
import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;
import org.openqa.selenium.By;
import org.openqa.selenium.WebDriver;
import org.openqa.selenium.WebElement;
import org.openqa.selenium.chrome.ChromeDriver;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class UVICHomepageTest {
    WebDriver driver;

    @Before
    public void setUp(){
        // Set up driver
        System.setProperty("webdriver.chrome.driver", "C:\\Users\\Amaan\\Desktop\\chromedriver.exe");
        driver = new ChromeDriver();
    }

    @Test
    public void OpenUVICWebsite(){
        // Open uvic home page
        driver.navigate().to("https://www.uvic.ca/");
        driver.manage().window().maximize();
        // Check title indicates it is the uvic homepage
        WebElement title = driver.findElement(By.xpath("//*[@id=\"uv-header\"]/div[2]/div/div/a"));
        assert(title.getText().toLowerCase().contains("university of victoria"));
    }

    @Test
    public void CheckNavBarItems(){
        // Open uvic home page
        driver.navigate().to("https://www.uvic.ca/");
        driver.manage().window().maximize();
        // Collect the nav bar items from the website
        WebElement nav_bar = driver.findElement(By.xpath("//*[@id=\"uvic-nav\"]"));
        List<WebElement> nav_elements = nav_bar.findElements(By.cssSelector("a"));
        List<String> actualList = new ArrayList<String>();
        for (WebElement nav_item: nav_elements) {
            if(!nav_item.getText().equals("")){
                actualList.add(nav_item.getText());
            }
        }
        //Compare encountered list vs. expected list
        List<String> expectedList = Arrays.asList("Admissions", "Academics", "Research", "Campus", "About UVic", "Support UVic");
        assert(expectedList.size() == actualList.size());
        for (int index = 0; index < expectedList.size(); index++){
            assert(actualList.get(index).contains(expectedList.get(index)));
        }
    }

    @Test
    public void VerifySignInButton(){
        // Open uvic home page
        driver.navigate().to("https://www.uvic.ca/");
        driver.manage().window().maximize();
        // Locate sign in button and check text to verify it is there
        WebElement button = driver.findElement(By.xpath("//*[@id=\"uv-header\"]/div[1]/div[2]/div/div/div[2]/a[2]"));
        assert(button.getText().toLowerCase().contains("sign in"));
    }

    @Test
    public void FindContactNumber(){
        // Open uvic home page
        driver.navigate().to("https://www.uvic.ca/");
        driver.manage().window().maximize();
        // Locate the phone number on the website and verify it is correct
        WebElement phoneNumber = driver.findElement(By.xpath("/html/body/footer/div/div[3]/div/div/div[2]/div/div[1]/ul/li[1]/a"));
        assert(phoneNumber.getText().contains("1-250-721-7211"));
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
