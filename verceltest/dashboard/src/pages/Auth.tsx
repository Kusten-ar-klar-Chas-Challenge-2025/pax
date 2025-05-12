const Auth: React.FC = () => {
  return (
    <main className="flex items-center justify-center min-h-screen bg-[#10302B] p-6">
      <div className="bg-white/10 rounded-lg p-8 w-full max-w-md backdrop-blur-sm">
        <div className="flex justify-center mb-5">
          <img src="/pax.logo.png" alt="Pax logo" className="w-32" />
        </div>

        <h1 className="text-2xl font-bold text-white">Logga in</h1>
        <form className="mt-4">
          <div className="mb-4">
            <label
              htmlFor="username"
              className="block text-sm font-medium text-white"
            >
              Användarnamn
            </label>
            <input
              type="text"
              id="username"
              name="username"
              className="mt-1 block w-full p-2 rounded-md border-gray-300 shadow-sm focus:ring-indigo-500 focus:border-[#10302B] sm:text-sm"
            />
          </div>
          <div className="mb-4">
            <label
              htmlFor="password"
              className="block text-sm font-medium text-white"
            >
              Lösenord
            </label>
            <input
              type="password"
              id="password"
              name="password"
              className="mt-1 block w-full p-2 rounded-md border-gray-300 shadow-sm focus:ring-indigo-500 focus:border-[#10302B] sm:text-sm"
            />
          </div>
          <button
            type="submit"
            className="w-full bg-[#7DBA6A] text-black font-semibold py-2 px-4 rounded-md hover:bg-[#B5DA87] transition"
          >
            Logga in
          </button>
        </form>
      </div>
    </main>
  );
};

export default Auth;
