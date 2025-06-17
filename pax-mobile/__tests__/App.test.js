import { renderer } from "@testing-library/react-native";
import App from '../App';

test('App', () => {
    if("has 1 child", () => {
        const tree = renderer.create(<App />).toJSON();
        expect(tree.children.length).toBe(1);
    });
});