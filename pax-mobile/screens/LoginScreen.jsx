import { ScrollView, StyleSheet, Text, View } from 'react-native';
import React, { useEffect, useState } from 'react';
import FormComponent from '../components/FormComponent';
import LogoComponent from '../components/LogoComponent';
import { useTheme } from '../theme/ThemeContext';
import ThemeToggleTabButton from '../components/ThemeToggleTabButton';
import ButtonComponent from '../components/buttons/ButtonComponent';
import { useNavigation } from '@react-navigation/native';
import { useUser } from '../context/UserContext';
import { useAuth } from '../context/AuthContext';
import { loginWithApi } from '../services/api';

const LoginScreen = ({ }) => {
  const { theme } = useTheme();
  const styles = createStyles(theme);
  const { login } = useAuth();
  const navigation = useNavigation();

  const [email, setEmail] = useState('');
  const [password, setPassword] = useState('');
  const [error, setError] = useState('');

  const handleLogin = async () => {
    if (!email || !password) {
      setError('Fyll i användarnamn och lösenord.');
      return; 
    }

    try {
      const result = await loginWithApi(email, password);

      if (!result || !result.token) {
        setError('Felaktiga inloggningsuppgifter');
        return;
      }
      await login(result.token);

    } catch (error) {
      console.error ('Login error', error);
      setError ('Något gick fel vid inloggning')
    }
  };

  return (
    <ScrollView style={styles.scrollcontainer}>
      <View style={styles.container}>
        <ThemeToggleTabButton />
        <LogoComponent style={styles.logo} />
        <FormComponent
          email={email}
          setEmail={setEmail}
          password={password}
          setPassword={setPassword}
        />
         {error ? <Text style={styles.error}>{error}</Text> : null}
        <ButtonComponent
          title="Logga in"
          onPress={handleLogin}
          style={({ pressed }) => [styles.button, pressed && styles.pressed]}/> 
      </View>
      </ScrollView>
  );
};

export default LoginScreen;

const createStyles = (theme) => 
    StyleSheet.create({
      scrollcontainer: {
        flexGrow: 1,
        backgroundColor: theme.background,
      },
      container: {
        flex: 1,
        backgroundColor: theme.background,
        paddingBottom: 0,
      },
      logo: {
        marginTop: 50,
        alignSelf: 'center',
      },
      label: {
        fontSize: 20,
        textAlign: 'left',
        alignSelf: 'flex-start',
        marginLeft: 10,
        color: theme.textPrimary,

      },
      input: {
        padding: 20,
        width: 300,
        backgroundColor: theme.card,
        borderRadius: 10,
        margin: 10,
      },
      button: {
        padding: 15,
        backgroundColor: theme.card,
        borderRadius: 10,
        width: 100,
        textAlign: 'right',
        alignSelf: 'flex-end',
      },
      pressed: {
        opacity: 0.2,
      },
      error: {
        color: theme.textPrimary,
        fontSize: 20,
        marginLeft: 10,
      }
    })