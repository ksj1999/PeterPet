class CalorieCalculator {
    constructor(acttime, dogid, weight, bcs) {
        this.time = acttime;
        this.id = dogid;
        this.rer = Math.round(70 * Math.pow(weight, 0.75) * 100) / 100;
        this.bcs = bcs;
    }

    calculateDer(nowKcal) {
        let actLevel;
        if (nowKcal < this.rer / 5) {
            actLevel = 0;
        } else if (nowKcal >= this.rer / 5 && nowKcal < this.rer / 3) {
            actLevel = 1;
        } else if (nowKcal >= this.rer / 3) {
            actLevel = 2;
        }

        let der;
        if (this.bcs === 'Obesity') {
            der = 1.4 * this.rer;
        } else if (this.bcs === 'Underweight') {
            der = 2 * this.rer;
        } else if (this.bcs === 'Normal') {
            switch (actLevel) {
                case 0:
                    der = 1.8 * this.rer; // Low activity
                    break;
                case 1:
                    der = 2.0 * this.rer; // Medium activity
                    break;
                case 2:
                    der = 3.5 * this.rer; // High activity
                    break;
            }
        }

        return [der, actLevel];
    }
}

class FeedManager {
    constructor(acttime, dogid, weight, bcs, nowKcal) {
        this.calorieCalculator = new CalorieCalculator(acttime, dogid, weight, bcs);
        this.calDer = this.calorieCalculator.calculateDer(nowKcal)[0];
    }

    dailyAmount(feed) {
        const amountFeed = this.calDer / feed * 1000;
        return amountFeed;
    }
}

module.exports = { CalorieCalculator, FeedManager };
