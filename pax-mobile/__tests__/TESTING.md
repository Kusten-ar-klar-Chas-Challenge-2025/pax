# TESTING

## 1. Teststrategi
Vi testar olika komponenter och funktioner för att enklare kunna upptäcka fel och brister. Vi ska göra två test vardera. 


## 2. Testmiljö 
- **Testramverk**: Jest 

## 3. Installation och körning

För att installera kör: 

```bash
npx expo install jest-expo jest

```
Addera sedan följande i package.json: 
```bash
"scripts": {
  ...
  "test": "jest"
},
"jest": {
  "preset": "jest-expo"
}
```
Skapa sedan en_ _ _tests_ _ _mapp med en Example-test.js och addera denna kod:

```bash
it('works', () => {
    expect(1).toBe(1);
});
```
Kör detta kommando i terminalen :
```bash
npm test
```


### 3.1 Installera beroenden 