import React from 'react';
import { render } from '@testing-library/react-native';
import { Text } from 'react-native';

test('visar text korrekt', () => {
    const { getByText } = render(<Text>Hello world</Text>);
    expect(getByText('Hello world')).toBeTruthy();
});
