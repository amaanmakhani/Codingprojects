package com.gildedrose;

import org.junit.jupiter.api.Disabled;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;

class GildedRoseTest {

    // Purpose is to test that the quality value is never negative
    @Test
    void QualityValueNonNegative() {
        Item[] items = new Item[] { new Item("foo", 10, 9) };
        GildedRose app = new GildedRose(items);
        nDaysPassed(app,10);
        assertEquals(0,app.items[0].quality);
    }

    // Purpose is to ensure the quality decreases by 1 when sellin period hasn't finished and twice as fast after the selling period has expired
    @Test
    void QualityDecreases(){
        Item[] items = new Item[] { new Item("foo1", 4, 10) };
        GildedRose app = new GildedRose(items);
        nDaysPassed(app,6);
        assertEquals(2,app.items[0].quality);
    }

    // Purpose is to ensure the quality can't go over 50
    @Test
    void QualityCapped() {
        Item[] items = new Item[]{new Item("Aged Brie", 10, 41)};
        GildedRose app = new GildedRose(items);
        nDaysPassed(app, 10);
        assertEquals(50, app.items[0].quality);
    }

    //Ex. 1, ensure once the sellin date expires the quality decreases twice as fast
    @Test
    void QualityDecreasesTwiceAsFast() {
        Item[] items = new Item[]{new Item("test", 12, 29)};
        GildedRose app = new GildedRose(items);
        nDaysPassed(app,15);
        assertEquals(11, app.items[0].quality);
    }

    //Ex. 2, ensure the quality of Sulfuras never changes, uses max int value based on inspection of days type
    @Test
    void QualityOfSulfurasDoesntChange() {
        Item[] items = new Item[]{new Item("Sulfuras, Hand of Ragnaros", 13, 80)};
        GildedRose app = new GildedRose(items);
        nDaysPassed(app, Integer.MAX_VALUE);
        assertEquals(80, app.items[0].quality);
    }

    // Ex. 3, the answer should be 29 + (1 * 3) + (2 * 5) + (3 * 4) = 29 + 25 = 54
    // Since quality is capped at 50 this test should come out to be 50
    @Test
    void BackstagePassQuality() {
        Item[] items = new Item[]{new Item("Backstage passes to a TAFKAL80ETC concert", 13, 29)};
        GildedRose app = new GildedRose(items);
        nDaysPassed(app, 12);
        assertEquals(50, app.items[0].quality);
    }

    // Helper function that was modified to use a variable amount of days rather than a fixed amount.
    private void nDaysPassed(GildedRose app,int days){
        assert days >= 0;
        for (int i=0;i<days;i++)
            app.updateQuality();
    }
}
